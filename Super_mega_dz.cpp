// Авиационный профиль: Моделирование вертикальной траектории
// Версия 1.2 | Последнее обновление: 23.10.2023

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <memory>
#include <cstdio>
#include <stdio.h> 
#include <stdlib.h> 
#include <locale.h>
#define M_PI 3.14159

// ------------------------------------------------------------------
// КОНСТАНТЫ ДЛЯ МОДЕЛИ ТУ-154
// ------------------------------------------------------------------
const double MASS_BASELINE = 43000.0;      // базовая масса, кг
const double WING_SPAN_AREA = 127.3;        // площадь крыла, м²
const double THRUST_TOTAL = 2*6800.0;     // суммарная тяга двигателей, Н
const double THROTTLE_SETTING = 1.;         // положение РУД
const double ALT_START = 300.0;        // начальная высота, м
const double ALT_TARGET = 6000.0;       // целевая высота, м
const double VEL_INITIAL_KPH = 310.0;        // начальная скорость, км/ч
const double VEL_TARGET_KPH = 700.0;        // целевая скорость, км/ч
const double G_CONST = 9.81;         // ускорение свободного падения
const double R_GAS_AIR = 287.05;       // газовая постоянная воздуха

// Преобразование скоростей в м/с
const double VEL_INITIAL_MS = VEL_INITIAL_KPH / 3.6;
const double VEL_TARGET_MS = VEL_TARGET_KPH / 3.6;

// ------------------------------------------------------------------
// КЛАСС ДЛЯ РАБОТЫ С CSV-ФАЙЛАМИ
// ------------------------------------------------------------------
class CSVExport {
    std::ofstream outputStream;

public:
    explicit CSVExport(const std::string& fname) {
        outputStream.open(fname);
        if (!outputStream.is_open()) {
            throw std::runtime_error("Не удалось открыть файл " + fname);
        }
    }

    ~CSVExport() {
        if (outputStream.is_open()) {
            outputStream.close();
        }
    }

    void writeHeaders(const std::vector<std::string>& headers) {
        for (size_t idx = 0; idx < headers.size(); ++idx) {
            outputStream << headers[idx];
            if (idx != headers.size() - 1) outputStream << ',';
        }
        outputStream << '\n';
    }

    void addDataLine(const std::vector<double>& values) {
        for (size_t idx = 0; idx < values.size(); ++idx) {
            outputStream << std::setprecision(6) << values[idx];
            if (idx != values.size() - 1) outputStream << ',';
        }
        outputStream << '\n';
    }
};

// ------------------------------------------------------------------
// ИНТЕРПОЛЯТОР ДАННЫХ СТАНДАРТНОЙ АТМОСФЕРЫ
// ------------------------------------------------------------------
class AtmosphereData {
    std::vector<double> hVec;      // высоты, м
    std::vector<double> tVec;      // температуры, К
    std::vector<double> pVec;      // давления, Па
    std::vector<double> rhoVec;    // плотности, кг/м³
    std::vector<double> aVec;      // скорости звука, м/с

public:
    AtmosphereData() {
        loadReferenceData();
    }

private:
    void loadReferenceData() {
        // Табличные данные МСА
        hVec = {0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000,  5500,   6000};
        tVec = {288.15, 284.90, 281.65, 278.40, 275.15, 271.90, 268.65, 265.40, 262.15, 258.90, 255.65, 252.40, 249.15  };
        pVec = {101325.0, 95463.0, 89976.0, 84808.0, 79916.0, 75260.0, 70801.0, 66604.0, 62634.0, 58860.0, 55252.0, 51881.0, 48720.0};
        rhoVec = {1.2250, 1.1673, 1.1117, 1.0581, 1.0066, 0.9570, 0.9093, 0.8635, 0.8196, 0.7775, 0.7369, 0.6979, 0.6605};
        aVec = {340.29, 338.37, 336.44, 334.50, 332.54, 330.57, 328.58, 326.58, 324.56, 322.52, 320.47, 318.40, 316.31};
    }

    double linearInterp(double x, const std::vector<double>& xs,
        const std::vector<double>& ys) const {
        if (x <= xs[0]) return ys[0];
        if (x >= xs.back()) return ys.back();

        for (size_t i = 0; i < xs.size() - 1; ++i)
            {
                    if (x >= xs[i] && x <= xs[i + 1]) {
                        double frac = (x - xs[i]) / (xs[i + 1] - xs[i]);
                        return ys[i] + frac * (ys[i + 1] - ys[i]);
                    }
            }
        return ys.back();
    }

public:
    double temperature(double altitude) const {
        return linearInterp(altitude, hVec, tVec);
    }

    double pressure(double altitude) const {
        return linearInterp(altitude, hVec, pVec);
    }

    double density(double altitude) const {
        return linearInterp(altitude, hVec, rhoVec);
    }

    double soundSpeed(double altitude) const {
        return linearInterp(altitude, hVec, aVec);
    }

    double machNumber(double velocity, double altitude) const {
        return velocity / soundSpeed(altitude);
    }
};

// ------------------------------------------------------------------
// СТРУКТУРА ДАННЫХ ТОЧКИ ТРАЕКТОРИИ
// ------------------------------------------------------------------
struct FlightState {
    double t;           // время, с
    double x;           // горизонтальная координата, м
    double h;           // высота, м
    double V;           // полная скорость, м/с
    double Vh;          // горизонтальная составляющая скорости, м/с
    double Vv;          // вертикальная составляющая скорости, м/с
    double theta;       // угол траектории, рад
    double alpha;       // угол атаки, рад
    double fuelUsed;    // израсходованное топливо, кг
    double massCurr;    // текущая масса, кг
    double accel;       // ускорение, м/с²
    double mach;        // число Маха

    FlightState(double time = 0, double posX = 0, double altitude = 0,
        double speed = 0, double speedH = 0, double speedV = 0,
        double pathAngle = 0, double aoa = 0, double fuel = 0,
        double mass = MASS_BASELINE, double acceleration = 0,
        double machNum = 0)
        : t(time), x(posX), h(altitude), V(speed), Vh(speedH), Vv(speedV),
        theta(pathAngle), alpha(aoa), fuelUsed(fuel), massCurr(mass),
        accel(acceleration), mach(machNum) {}

    void display() const {
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "Время: " << t << "с | Высота: " << h << "м | ";
        std::cout << "Скорость: " << V * 3.6 << "км/ч | ";
        std::cout << "Вертикальная скорость: " << Vv << "м/с | ";
        std::cout << "Угол: " << theta * 180 / M_PI << "° | ";
        std::cout << "Масса: " << massCurr << "кг\n";
    }
};

// ------------------------------------------------------------------
// КЛАСС ДЛЯ ХРАНЕНИЯ ТРАЕКТОРИИ ПОЛЕТА
// ------------------------------------------------------------------
class FlightPath {
    std::vector<FlightState> pathPoints;

public:
    FlightPath() = default;

    void appendPoint(const FlightState& point) {
        pathPoints.push_back(point);
    }

    const std::vector<FlightState>& getAllPoints() const {
        return pathPoints;
    }

    double totalDuration() const {
        return pathPoints.empty() ? 0.0 : pathPoints.back().t;
    }

    double totalFuelConsumed() const {
        return pathPoints.empty() ? 0.0 : pathPoints.back().fuelUsed;
    }

    void exportToCSV(const std::string& filename) {
        CSVExport csvFile(filename);

        csvFile.writeHeaders({
            "time_s", "altitude_m", "velocity_ms", "velocity_kmh",
            "vertical_velocity_ms", "theta_deg", "alpha_deg",
            "fuel_kg", "mass_kg", "acceleration_ms2", "mach_number"
            });

        for (const auto& pt : pathPoints) {
            csvFile.addDataLine({
                pt.t, pt.h, pt.V, pt.V * 3.6,
                pt.Vv, pt.theta * 180 / M_PI, pt.alpha * 180 / M_PI,
                pt.fuelUsed, pt.massCurr, pt.accel, pt.mach
                });
        }

        std::cout << "Экспорт завершен. Файл: " << filename << '\n';
    }

    void visualize() const {
        // Создаем временный файл с данными для построения
        const std::string dataFile = "temp_plot_data.dat";
        std::ofstream dataStream(dataFile);

        if (!dataStream.is_open()) {
            std::cerr << "Не удалось создать временный файл данных\n";
            return;
        }

        // Записываем данные в формате: время высота скорость_кмч
        for (const auto& pt : pathPoints) {
            dataStream << std::fixed << std::setprecision(2)
                << pt.t << " " << pt.h << " " << pt.V * 3.6 << "\n";
        }
        dataStream.close();

#ifdef _WIN32
        // Для Windows - создаем файл скрипта Gnuplot и предлагаем пользователю запустить его
        createWindowsGnuplotScript(dataFile);
        std::cout << "\nДля визуализации:\n";
        std::cout << "1. Установите Gnuplot (http://www.gnuplot.info)\n";
        std::cout << "2. Запустите файл plot_script.plt\n";
        std::cout << "Или откройте CSV файл в Excel/Google Sheets\n";
#else
        // Для Linux/Mac используем popen
        FILE* gnuplotPipe = popen("gnuplot -persistent", "w");
        if (!gnuplotPipe) {
            std::cerr << "Ошибка инициализации Gnuplot\n";
            std::cout << "Данные для построения сохранены в " << dataFile << "\n";
            return;
        }

        fprintf(gnuplotPipe, "set terminal wxt size 1200,800 font 'Arial,12'\n");
        fprintf(gnuplotPipe, "set multiplot layout 2,2 title 'Динамика полета ТУ-154'\n");

        // График высоты
        fprintf(gnuplotPipe, "set title 'Изменение высоты'\n");
        fprintf(gnuplotPipe, "set xlabel 'Время, с'\n");
        fprintf(gnuplotPipe, "set ylabel 'Высота, м'\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "plot '%s' using 1:2 with lines lw 2 lc 'blue' title 'Высота'\n",
            dataFile.c_str());

        // График скорости
        fprintf(gnuplotPipe, "set title 'Изменение скорости'\n");
        fprintf(gnuplotPipe, "set xlabel 'Время, с'\n");
        fprintf(gnuplotPipe, "set ylabel 'Скорость, км/ч'\n");
        fprintf(gnuplotPipe, "set grid\n");
        fprintf(gnuplotPipe, "plot '%s' using 1:3 with lines lw 2 lc 'red' title 'Скорость'\n",
            dataFile.c_str());

        fprintf(gnuplotPipe, "unset multiplot\n");
        fflush(gnuplotPipe);

        std::cout << "Для продолжения закройте окно Gnuplot...\n";
        std::cin.get();

        pclose(gnuplotPipe);
#endif

        // Удаляем временный файл
        std::remove(dataFile.c_str());
    }

#ifdef _WIN32
    void createWindowsGnuplotScript(const std::string& dataFile) const {
        std::ofstream script("plot_script.plt");
        if (!script.is_open()) return;

        script << "# Скрипт Gnuplot для визуализации траектории\n";
        script << "set terminal png size 1200,800 enhanced font 'Arial,12'\n";
        script << "set output 'flight_trajectory.png'\n";
        script << "set multiplot layout 2,2 title 'Динамика полета ТУ-154'\n\n";

        script << "# График высоты\n";
        script << "set title 'Изменение высоты'\n";
        script << "set xlabel 'Время, с'\n";
        script << "set ylabel 'Высота, м'\n";
        script << "set grid\n";
        script << "plot '" << dataFile << "' using 1:2 with lines lw 2 lc 'blue' title 'Высота'\n\n";

        script << "# График скорости\n";
        script << "set title 'Изменение скорости'\n";
        script << "set xlabel 'Время, с'\n";
        script << "set ylabel 'Скорость, км/ч'\n";
        script << "set grid\n";
        script << "plot '" << dataFile << "' using 1:3 with lines lw 2 lc 'red' title 'Скорость'\n\n";

        script << "unset multiplot\n";
        script << "print 'График сохранен в файл flight_trajectory.png'\n";
        script << "pause -1\n";

        script.close();
    }
#endif
};

// ------------------------------------------------------------------
// МОДЕЛЬ ЛЕТАТЕЛЬНОГО АППАРАТА
// ------------------------------------------------------------------
class AircraftModel {
    AtmosphereData atmosphere;
    double wingArea;
    double massInitial;

public:
    double massCurrent;
    double thrust;
    double fuelBurnRate;
    double dragCoeffZero;
    double inducedDragCoeff;
    double maxLiftCoeff;

    AircraftModel(double mass = MASS_BASELINE, double area = WING_SPAN_AREA)
        : wingArea(area), massInitial(mass), massCurrent(mass) {

        thrust = THRUST_TOTAL * THROTTLE_SETTING;
        fuelBurnRate = 2.5;                // кг/с
        dragCoeffZero = 0.02;
        inducedDragCoeff = 0.05;
        maxLiftCoeff = 1.2;
    }

    double computeLiftCoeff(double aoa) const {
        const double LIFT_SLOPE = 5.0;
        return std::min(LIFT_SLOPE * aoa, maxLiftCoeff);
    }

    double computeDragCoeff(double liftCoeff) const {
        return dragCoeffZero + inducedDragCoeff * liftCoeff * liftCoeff;
    }

    double calculateLift(double velocity, double altitude, double aoa) const {
        double airDensity = atmosphere.density(altitude);
        double dynamicPress = 0.5 * airDensity * velocity * velocity;
        double cl = computeLiftCoeff(aoa);
        return cl * wingArea * dynamicPress;
    }

    double calculateDrag(double velocity, double altitude, double aoa) const {
        double airDensity = atmosphere.density(altitude);
        double dynamicPress = 0.5 * airDensity * velocity * velocity;
        double cl = computeLiftCoeff(aoa);
        double cd = computeDragCoeff(cl);
        return cd * wingArea * dynamicPress;
    }

    FlightState propagateState(const FlightState& current,
        double timeStep,
        double commandedAOA) {
        FlightState next = current;

        // Ограничение угла атаки
        commandedAOA = std::max(-0.1, std::min(0.2, commandedAOA));

        // Расчет аэродинамических сил
        double liftForce = calculateLift(current.V, current.h, commandedAOA);
        double dragForce = calculateDrag(current.V, current.h, commandedAOA);

        // Уравнения движения
        double forceX = thrust - dragForce - massCurrent * G_CONST * sin(current.theta);
        double forceY = liftForce - massCurrent * G_CONST * cos(current.theta);

        double accelX = forceX / massCurrent;
        double accelY = forceY / massCurrent;

        // Интегрирование скорости
        next.V = current.V + accelX * timeStep;
        next.V = std::max(100.0, next.V);  // минимальная скорость

        if (next.V > 0) {
            next.theta = atan2(accelY, accelX + G_CONST * sin(current.theta));
        }

        next.theta = std::max(-0.3, std::min(0.3, next.theta));
        next.Vh = next.V * cos(next.theta);
        next.Vv = next.V * sin(next.theta);

        // Интегрирование координат
        next.x = current.x + next.Vh * timeStep;
        next.h = current.h + next.Vv * timeStep;

        // Защита от отрицательной высоты
        if (next.h < 0) {
            next.h = 10.0;
            next.Vv = std::max(0.0, next.Vv);
        }

        // Обновление параметров
        next.alpha = commandedAOA;
        next.mach = atmosphere.machNumber(next.V, next.h);
        next.accel = accelX;

        double deltaFuel = fuelBurnRate * timeStep;
        next.fuelUsed = current.fuelUsed + deltaFuel;
        next.massCurr = current.massCurr - deltaFuel;

        // Коррекция тяги (упрощенная модель)
        thrust = THRUST_TOTAL * THROTTLE_SETTING * (next.massCurr / massInitial);

        next.t = current.t + timeStep;

        return next;
    }

    double initialMass() const { return massInitial; }
};

// ------------------------------------------------------------------
// АЛГОРИТМ ОПТИМИЗАЦИИ ТРАЕКТОРИИ
// ------------------------------------------------------------------
class TrajectoryOptimizer {
public:
    TrajectoryOptimizer() = default;

    FlightPath computeOptimalPath(AircraftModel& aircraft,
        double maxTime = 600.0) {
        FlightPath resultPath;
        const double TIME_STEP = 1.0;

        // Начальные условия
        FlightState initialState;
        initialState.t = 0;
        initialState.x = 0;
        initialState.h = ALT_START;
        initialState.V = VEL_INITIAL_MS;
        initialState.Vh = VEL_INITIAL_MS;
        initialState.Vv = 5.0;
        initialState.theta = 0.05;
        initialState.alpha = 0.03;
        initialState.fuelUsed = 0;
        initialState.massCurr = aircraft.initialMass();
        initialState.accel = 0;

        AtmosphereData envCalc;
        initialState.mach = envCalc.machNumber(initialState.V, initialState.h);

        FlightState currentState = initialState;
        resultPath.appendPoint(currentState);

        std::cout << "\n=== ПАРАМЕТРЫ МОДЕЛИРОВАНИЯ ===\n";
        std::cout << "Целевая высота: " << ALT_TARGET << " м\n";
        std::cout << "Целевая скорость: " << VEL_TARGET_KPH << " км/ч\n";
        std::cout << "Максимальное время: " << maxTime << " с\n\n";

        int iteration = 0;
        bool targetAchieved = false;

        while (currentState.t < maxTime && !targetAchieved) {
            iteration++;

            // Простой закон управления углом атаки
            double heightFraction = currentState.h / ALT_TARGET;
            double aoaCommand;

            if (heightFraction < 0.3) {
                aoaCommand = 0.06;          // активный набор высоты
            }
            else if (heightFraction < 0.7) {
                aoaCommand = 0.04;          // переходный режим
            }
            else {
                aoaCommand = 0.02;          // вывод на заданную высоту
            }

            // Коррекция для набора скорости
            if (currentState.V < VEL_TARGET_MS * 0.9) {
                aoaCommand -= 0.01;
            }

            currentState = aircraft.propagateState(currentState,
                TIME_STEP,
                aoaCommand);
            resultPath.appendPoint(currentState);

            // Периодический вывод информации
            if (iteration % 30 == 0) {
                std::cout << "Шаг " << iteration << " | ";
                std::cout << "Время: " << currentState.t << "с | ";
                std::cout << "Высота: " << currentState.h << "м | ";
                std::cout << "Скорость: " << currentState.V * 3.6 << "км/ч\n";
            }

            // Проверка достижения цели
            if (currentState.h >= ALT_TARGET) {
                targetAchieved = true;
                std::cout << "\n>>> ЦЕЛЕВАЯ ВЫСОТА ДОСТИГНУТА! <<<\n";
            }

            // Проверка на нефизичные значения
            if (currentState.h > 20000 || currentState.V > 1000) {
                std::cout << "\n!!! Прерывание: выход за допустимые пределы\n";
                break;
            }
        }

        // Итоговый отчет
        std::cout << "\n=== РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ ===\n";
        std::cout << "Финальная высота: " << currentState.h << " м ("
            << (currentState.h / ALT_TARGET * 100) << "% от цели)\n";
        std::cout << "Финальная скорость: " << currentState.V * 3.6 << " км/ч\n";
        std::cout << "Общее время: " << currentState.t << " с\n";
        std::cout << "Расход топлива: " << currentState.fuelUsed << " кг\n";
        std::cout << "Число Маха: " << currentState.mach << "\n";

        return resultPath;
    }
};

// ------------------------------------------------------------------
// ОСНОВНАЯ ФУНКЦИЯ
// ------------------------------------------------------------------
int main() {
    setlocale(LC_ALL, "ru");
    try {
        AircraftModel tu134Model;
        TrajectoryOptimizer optimizer;

        FlightPath trajectory = optimizer.computeOptimalPath(tu134Model, 300.0);

        trajectory.exportToCSV("flight_profile_tu154.csv");
        trajectory.visualize();

    }
    catch (const std::exception& err) {
        std::cerr << "КРИТИЧЕСКАЯ ОШИБКА: " << err.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}