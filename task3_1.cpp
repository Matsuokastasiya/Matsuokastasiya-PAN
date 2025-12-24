#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>  // Добавлено для std::max_element

int main() {
    // Константы
    const double v0 = 50.0;  // начальная скорость (м/с)
    const double g = 9.8;    // ускорение свободного падения (м/с²)
    const double dt = 1.0;   // шаг времени (с)
    
    // Проверка граничных случаев
    if (v0 <= 0.0) {
        std::cout << "Предупреждение: начальная скорость v0 должна быть положительной!" << std::endl;
        return 1;
    }
    
    // Создание векторов для хранения времени и высоты
    std::vector<double> times;
    std::vector<double> heights;
    
    // Резервирование памяти для оптимизации
    // Примерный размер: время подъема примерно равно v0*2/g (время подъема и падения)
    int estimated_size = static_cast<int>(v0 * 2 / g / dt) + 1;
    times.reserve(estimated_size);
    heights.reserve(estimated_size);
    
    // Расчет траектории
    for (double t = 0.0; t <= 1000.0; t += dt) {
        double h = v0 * t - 0.5 * g * t * t;
        
        // Если ракета упала на землю
        if (h < 0) {
            break;
        }
        
        // Добавление значений в векторы
        times.push_back(t);
        heights.push_back(h);
    }
    
    // Проверка, что есть данные для вывода
    if (times.empty()) {
        std::cout << "Нет данных для отображения." << std::endl;
        return 0;
    }
    
    // Вывод заголовка таблицы
    std::cout << std::string(30, '-') << std::endl;
    std::cout << std::setw(10) << "Время (с)" 
              << std::setw(20) << "Высота (м)" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    // Вывод данных таблицы
    for (size_t i = 0; i < times.size(); ++i) {
        std::cout << std::fixed << std::setprecision(1)
                  << std::setw(10) << times[i]
                  << std::setw(20) << std::setprecision(2) << heights[i]
                  << std::endl;
    }
    
    std::cout << std::string(30, '-') << std::endl;
    
    // Дополнительная информация
    std::cout << "\nДополнительная информация:" << std::endl;
    std::cout << "Количество точек: " << times.size() << std::endl;
    std::cout << "Время полета: " << times.back() << " с" << std::endl;
    std::cout << "Максимальная высота: " << *std::max_element(heights.begin(), heights.end()) << " м" << std::endl;
    
    return 0;
}