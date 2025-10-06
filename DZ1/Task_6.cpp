#include <iostream> // input-output library
#include <cmath> // include math
#include <string> 
#include <iomanip>

const double g = 9.81;

// Функция для расчета подъемной силы
double calculateLift(double rho, double v, double s, double cl) {
    return 0.5 * rho * pow(v, 2) * s * cl;
}

int main() {
    // Параметры самолета (константы)
    const double WING_AREA = 30.0;    // площадь крыла (м²)
    const double LIFT_COEFFICIENT = 0.8; // коэффициент подъемной силы C_L
    const int NUM_STEPS = 10;         // количество шагов
    
    // Массивы скоростей и плотностей на разных шагах траектории
    double velocities[NUM_STEPS] = {100, 110, 105, 95, 90, 85, 80, 75, 70, 65};
    double densities[NUM_STEPS] = {1.225, 1.112, 1.150, 1.008, 0.909, 0.819, 0.736, 0.660, 0.590, 0.525};
    
    double liftForces[NUM_STEPS];
    
    // Вычисление подъемных сил на каждом шаге
    for (int i = 0; i < NUM_STEPS; i++) {
        liftForces[i] = calculateLift(densities[i], velocities[i], WING_AREA, LIFT_COEFFICIENT);
    }
    
    // Вывод заголовка таблицы
    std::cout << "=== РАСЧЕТ ПОДЪЕМНОЙ СИЛЫ НА ТРАЕКТОРИИ ===" << std::endl;
    std::cout << "Параметры самолета:" << std::endl;
    std::cout << "Площадь крыла: " << WING_AREA << " м²" << std::endl;
    std::cout << "Коэффициент подъемной силы C_L: " << LIFT_COEFFICIENT << std::endl;
    std::cout << std::endl;
    
    // Вывод таблицы
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(10) << "Шаг" // setw(10) - set width 10 - ширина "ячейки" таблицы
              << std::setw(22) << "Скорость" 
              << std::setw(22) << "Плотность" 
              << std::setw(29) << "Подъемная сила" << std::endl;
    std::cout << std::setw(6) << "" 
              << std::setw(14) << "(м/с)" 
              << std::setw(20) << "(кг/м³)" 
              << std::setw(14) << "(Н)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    std::cout << std::fixed << std::setprecision(2);
    
    for (int i = 0; i < NUM_STEPS; i++) {
        std::cout << std::setw(6) << i + 1
                  << std::setw(12) << velocities[i]
                  << std::setw(14) << densities[i]
                  << std::setw(18) << liftForces[i] << std::endl;
    }
    
    std::cout << std::string(60, '-') << std::endl;
    
    return 0;
}