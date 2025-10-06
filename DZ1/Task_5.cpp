#include <iostream> // input-output library
#include <cmath> // iclude math
#include <string> 
#include <iomanip>


    const double g = 9.81;
    const double RHO = 1.225;

    // Структура для хранения параметров самолета
    struct Airplane {
       std::string name;        // название самолета
       double mass;        // масса (кг)
       double wing_area;   // площадь крыла (м²)
       double thrust;      // тяга (Н)
       double cl;          // коэффициент подъемной силы C_L
       double cd;          // коэффициент сопротивления C_D
       double velocity;    // скорость полета (м/с)
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

int main()
{
     const double TARGET_HEIGHT = 1000.0; // целевая высота в метрах
    
    // Создаем массив из трех самолетов с разными параметрами (можно было бы сделать заполнение массива оператором через консоль, но мне лень :р)
    Airplane airplanes[3] = {
        {"Самолет 1", 15000, 30, 50000, 0.8, 0.02, 100},
        {"Самолет 2", 12000, 25, 45000, 0.9, 0.015, 100},
        {"Самолет 3", 18000, 35, 60000, 0.7, 0.025, 100}
    };

    // Массивы для хранения результатов расчетов
    double liftForces[3];
    double dragForces[3];
    double verticalAccelerations[3];
    double timesToHeight[3];
    
    // Вычисляем характеристики для каждого самолета
    for (int i = 0; i < 3; i++) {
        Airplane& plane = airplanes[i];
        
        // Вычисляем подъемную силу и сопротивление
        liftForces[i] = calculateLift(RHO, plane.velocity, plane.wing_area, plane.cl);
        dragForces[i] = calculateDrag(RHO, plane.velocity, plane.wing_area, plane.cd);
        
        // Вычисляем вертикальное ускорение
        verticalAccelerations[i] = calculateVerticalAcceleration(liftForces[i], plane.mass);
        
        // Вычисляем время набора высоты
        timesToHeight[i] = calculateTimeToHeight(TARGET_HEIGHT, verticalAccelerations[i]);

         if (timesToHeight[i] > 0) {
            std::cout << "Время набора высоты " << TARGET_HEIGHT << " м: " 
                     << timesToHeight[i] << " с" << std::endl;
        } else {
            std::cout << "Самолет не может набрать высоту " << TARGET_HEIGHT << " м" << std::endl;
        }
    }
    
    // Определяем самый быстрый самолет
    int fastestIndex = -1;
    double minTime = 1e9;
    
    for (int i = 0; i < 3; i++) {
        if (timesToHeight[i] > 0 && timesToHeight[i] < minTime) {
            minTime = timesToHeight[i];
            fastestIndex = i;
        }
    }
    
    // Вывод итогового результата
    std::cout << "\n=== РЕЗУЛЬТАТ ===" << std::endl;
    if (fastestIndex != -1) {
        std::cout << "Самый быстрый самолет: " << airplanes[fastestIndex].name << std::endl;
        std::cout << "Время набора высоты " << TARGET_HEIGHT << " м: " << minTime << " с" << std::endl;
        
    return 0;
  }
}