#include <iostream> // input-output library
#include <cmath> // include math
#include <string> 
#include <iomanip>
#include <algorithm> // для сортировки

const double g = 9.81;
const double RHO = 1.225; // плотность воздуха

// Структура для хранения параметров самолета
struct Aircraft {
    std::string name;        // название самолета
    double mass;        // масса (кг)
    double wing_area;   // площадь крыла (м²)
    double thrust;      // тяга (Н)
    double cl;          // коэффициент подъемной силы C_L
    double cd;          // коэффициент сопротивления C_D
    double velocity;    // скорость полета (м/с)
    double lift;        // подъемная сила (Н) - вычисляемое
    double drag;        // сопротивление (Н) - вычисляемое
    double acceleration; // вертикальное ускорение (м/с²) - вычисляемое
    double time_to_height; // время набора высоты (с) - вычисляемое
};

// Функция для расчета подъемной силы
double calculateLift(double rho, double v, double s, double cl) {
    return 0.5 * rho * pow(v, 2) * s * cl;
}

// Функция для расчета сопротивления
double calculateDrag(double rho, double v, double s, double cd) {
    return 0.5 * rho * pow(v, 2) * s * cd;
}

// Функция для расчета вертикального ускорения
double calculateVerticalAcceleration(double lift, double mass) {
    return (lift - mass * g) / mass;
}

// Функция для расчета времени набора высоты
double calculateTimeToHeight(double h, double a_y) {
    if (a_y <= 0) {
        return -1; // возвращаем -1 если набор высоты невозможен
    }
    return sqrt(2 * h / a_y);
}

// Функция для сравнения самолетов по времени набора высоты (для сортировки)
bool compareByTime(const Aircraft& a, const Aircraft& b) {
    // Сначала самолеты, которые могут набрать высоту
    if (a.time_to_height > 0 && b.time_to_height <= 0) return true;
    if (a.time_to_height <= 0 && b.time_to_height > 0) return false;
    // Если оба могут или не могут, сортируем по времени
    return a.time_to_height < b.time_to_height;
}

int main() {
    const double TARGET_HEIGHT = 1000.0; // целевая высота в метрах
    
    // Создаем массив самолетов
    const int NUM_AIRCRAFT = 5;
    Aircraft aircrafts[NUM_AIRCRAFT] = {
        {"Боинг 737       ", 50000, 125, 120000, 0.8, 0.02, 100},
        {"Airbus A320     ", 48000, 122, 110000, 0.85, 0.018, 105},
        {"Истребитель F-16", 12000, 28, 80000, 1.2, 0.025, 150},
        {"Cessna 172      ", 1000, 16, 1800, 0.9, 0.03, 60},
        {"Ту-154          ", 55000, 202, 100000, 0.75, 0.022, 95}
    };
    
    std::cout << "=== ВЫЧИСЛЕНИЕ ВРЕМЕНИ НАБОРА ВЫСОТЫ ДЛЯ САМОЛЕТОВ ===" << std::endl;
    std::cout << "Целевая высота: " << TARGET_HEIGHT << " м" << std::endl;
    std::cout << std::endl;
    
    // Вычисляем характеристики для каждого самолета
    for (int i = 0; i < NUM_AIRCRAFT; i++) {
        Aircraft& plane = aircrafts[i];
        
        // Вычисляем подъемную силу и сопротивление
        plane.lift = calculateLift(RHO, plane.velocity, plane.wing_area, plane.cl);
        plane.drag = calculateDrag(RHO, plane.velocity, plane.wing_area, plane.cd);
        
        // Вычисляем вертикальное ускорение
        plane.acceleration = calculateVerticalAcceleration(plane.lift, plane.mass);
        
        // Вычисляем время набора высоты
        plane.time_to_height = calculateTimeToHeight(TARGET_HEIGHT, plane.acceleration);
    }
    
    // Сортируем самолеты по времени набора высоты
    std::sort(aircrafts, aircrafts + NUM_AIRCRAFT, compareByTime);
    
    // Вывод результатов в таблице
    std::cout << std::string(120, '-') << std::endl;
    std::cout << std::setw(15) << "Самолет" 
              << std::setw(20) << "Масса" 
              << std::setw(25) << "Скорость"
              << std::setw(25) << "Подъемная" 
              << std::setw(25) << "Ускорение"
              << std::setw(30) << "Время набора" 
              << std::setw(15) << "Статус" << std::endl;
    std::cout << std::setw(15) << "" 
              << std::setw(10) << "(кг)" 
              << std::setw(15) << "(м/с)"
              << std::setw(20) << "(кН)" 
              << std::setw(20) << "(м/с²)"
              << std::setw(25) << "высоты (с)" 
              << std::setw(15) << "" << std::endl;
    std::cout << std::string(120, '-') << std::endl;
    
    std::cout << std::fixed << std::setprecision(2);
    
    for (int i = 0; i < NUM_AIRCRAFT; i++) {
        const Aircraft& plane = aircrafts[i];
        
        std::cout << std::setw(1) << plane.name
                  << std::setw(10) << plane.mass
                  << std::setw(15) << plane.velocity
                  << std::setw(15) << plane.lift / 1000
                  << std::setw(15) << plane.acceleration;
        
        if (plane.time_to_height > 0) {
            std::cout << std::setw(15) << plane.time_to_height
                      << std::setw(20) << "УСПЕХ";
        } else {
            std::cout << std::setw(20) << "---"
                      << std::setw(15) << "НЕВОЗМОЖНО";
        }
        std::cout << std::endl;
    }
    
    std::cout << std::string(120, '-') << std::endl;
    
    // Детальная информация о самолетах
    std::cout << std::endl;
    std::cout << "=== ДЕТАЛЬНАЯ ИНФОРМАЦИЯ ===" << std::endl;
    
    for (int i = 0; i < NUM_AIRCRAFT; i++) {
        const Aircraft& plane = aircrafts[i];
        
        std::cout << std::endl << "--- " << plane.name << " ---" << std::endl;
        std::cout << "Площадь крыла: " << plane.wing_area << " м²" << std::endl;
        std::cout << "Тяга: " << plane.thrust << " Н" << std::endl;
        std::cout << "C_L: " << plane.cl << ", C_D: " << plane.cd << std::endl;
        std::cout << "Сопротивление: " << plane.drag << " Н" << std::endl;
        std::cout << "Вертикальное ускорение: " << plane.acceleration << " м/с²" << std::endl;
        
        if (plane.time_to_height > 0) {
            std::cout << "Время набора " << TARGET_HEIGHT << " м: " 
                     << plane.time_to_height << " с" << std::endl;
        } else {
            std::cout << "Самолет не может набрать высоту " << TARGET_HEIGHT << " м" << std::endl;
        }
    }
    
    // Вывод победителя
    std::cout << std::endl;
    std::cout << "=== РЕЗУЛЬТАТ ===" << std::endl;
    if (aircrafts[0].time_to_height > 0) {
        std::cout << "Самый быстрый самолет: " << aircrafts[0].name << std::endl;
        std::cout << "Время набора высоты: " << aircrafts[0].time_to_height << " с" << std::endl;
    } else {
        std::cout << "Ни один самолет не может набрать заданную высоту" << std::endl;
    }
    
    return 0;
}