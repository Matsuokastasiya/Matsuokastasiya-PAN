#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>

int main() {
    // Константы
    const double v0 = 50.0;      // начальная скорость (м/с)
    const double g = 9.8;        // ускорение свободного падения (м/с²)
    const double dt = 0.5;       // шаг времени (с) - можно изменить на 1.0 для более редких точек
    const double t_max = 15.0;   // максимальное время моделирования (с)
    const double v_threshold = -100.0; // порог для остановки (очень отрицательная скорость)
    
    // Проверка граничных случаев
    if (v0 <= 0.0) {
        std::cout << "Предупреждение: начальная скорость v0 должна быть положительной!" << std::endl;
        return 1;
    }
    
    // Создание векторов для хранения времени и скорости
    std::vector<double> times;
    std::vector<double> velocities;
    
    // Резервирование памяти для оптимизации
    int estimated_size = static_cast<int>(t_max / dt) + 2;
    times.reserve(estimated_size);
    velocities.reserve(estimated_size);
    
    // Расчет скорости по времени
    for (double t = 0.0; t <= t_max; t += dt) {
        double v = v0 - g * t;
        
        // Опциональная остановка, если скорость стала очень отрицательной
        if (v < v_threshold) {
            std::cout << "Остановка: скорость стала очень отрицательной (v < " 
                      << v_threshold << " м/с) при t = " << t << " с" << std::endl;
            break;
        }
        
        // Добавление значений в векторы
        times.push_back(t);
        velocities.push_back(v);
    }
    
    // Проверка наличия данных
    if (times.empty()) {
        std::cout << "Нет данных о скорости." << std::endl;
        return 0;
    }
    
    // Вывод теоретической информации
    std::cout << "=== Теоретические расчеты ===" << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Начальная скорость v0: " << v0 << " м/с" << std::endl;
    std::cout << "Ускорение свободного падения g: " << g << " м/с²" << std::endl;
    std::cout << "Шаг времени dt: " << dt << " с" << std::endl;
    
    double t_zero = v0 / g;  // Время, когда скорость становится нулевой
    std::cout << "Время изменения знака скорости (t = v0/g): " << t_zero << " с" << std::endl;
    std::cout << "Скорость в момент t = v0/g: " << (v0 - g * t_zero) << " м/с" << std::endl;
    
    std::cout << "\n=== Таблица скоростей ===" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << std::setw(12) << "Время t (с)" 
              << std::setw(15) << "Скорость v(t)" 
              << std::setw(15) << "Знак" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    // Вывод данных таблицы
    for (size_t i = 0; i < times.size(); ++i) {
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(12) << times[i]
                  << std::setw(15) << velocities[i];
        
        // Определение знака скорости
        if (velocities[i] > 0.0) {
            std::cout << std::setw(15) << "+";
        } else if (velocities[i] < 0.0) {
            std::cout << std::setw(15) << "-";
        } else {
            std::cout << std::setw(15) << "0";
        }
        std::cout << std::endl;
        
        // Выделение момента изменения знака скорости
        if (i > 0 && velocities[i-1] > 0.0 && velocities[i] <= 0.0) {
            std::cout << std::string(40, '~') << std::endl;
            std::cout << "↑ Момент изменения знака скорости (около t = " 
                      << times[i] << " с)" << std::endl;
            std::cout << std::string(40, '~') << std::endl;
        }
    }
    
    std::cout << std::string(40, '-') << std::endl;
    
    // Дополнительная статистика
    std::cout << "\n=== Статистика ===" << std::endl;
    std::cout << "Количество точек: " << times.size() << std::endl;
    
    // Нахождение максимальной и минимальной скорости
    auto max_iter = std::max_element(velocities.begin(), velocities.end());
    auto min_iter = std::min_element(velocities.begin(), velocities.end());
    
    std::cout << "Максимальная скорость: " << *max_iter << " м/с (начальная)" << std::endl;
    std::cout << "Минимальная скорость: " << *min_iter << " м/с" << std::endl;
    
    // Поиск момента, когда скорость становится отрицательной
    auto first_negative = std::find_if(velocities.begin(), velocities.end(), 
                                       [](double v) { return v < 0.0; });
    
    if (first_negative != velocities.end()) {
        size_t idx = std::distance(velocities.begin(), first_negative);
        std::cout << "Первая отрицательная скорость: " << *first_negative 
                  << " м/с при t = " << times[idx] << " с" << std::endl;
    } else {
        std::cout << "В пределах моделирования скорость не стала отрицательной" << std::endl;
    }
    
    // Поиск момента, когда скорость ближе всего к нулю
    size_t idx_closest_to_zero = 0;
    double min_diff = fabs(velocities[0]);
    for (size_t i = 1; i < velocities.size(); ++i) {
        double diff = fabs(velocities[i]);
        if (diff < min_diff) {
            min_diff = diff;
            idx_closest_to_zero = i;
        }
    }
    
    std::cout << "Ближайшая к нулю скорость: " << velocities[idx_closest_to_zero] 
              << " м/с при t = " << times[idx_closest_to_zero] << " с" << std::endl;
    
    // Визуализация изменения скорости
    std::cout << "\n=== Визуализация изменения скорости ===" << std::endl;
    std::cout << "Время (с)  : ";
    for (size_t i = 0; i < std::min((size_t)10, times.size()); ++i) {
        std::cout << std::setw(6) << times[i];
    }
    if (times.size() > 10) std::cout << " ...";
    std::cout << std::endl;
    
    std::cout << "Скорость   : ";
    for (size_t i = 0; i < std::min((size_t)10, velocities.size()); ++i) {
        std::cout << std::setw(6) << std::setprecision(1) << velocities[i];
    }
    if (velocities.size() > 10) std::cout << " ...";
    std::cout << std::endl;
    
    std::cout << "Изменение  : ";
    for (size_t i = 0; i < std::min((size_t)10, velocities.size()); ++i) {
        if (i == 0) {
            std::cout << std::setw(6) << "начало";
        } else {
            double delta = velocities[i] - velocities[i-1];
            std::cout << std::setw(6) << std::setprecision(1) << delta;
        }
    }
    if (velocities.size() > 10) std::cout << " ...";
    std::cout << std::endl;
    
    // Проверка теоретических расчетов
    std::cout << "\n=== Проверка теоретических расчетов ===" << std::endl;
    std::cout << "Теоретическое время остановки (v=0): t = v0/g = " << t_zero << " с" << std::endl;
    
    // Линейная интерполяция для нахождения времени, когда скорость = 0
    for (size_t i = 1; i < velocities.size(); ++i) {
        if (velocities[i-1] >= 0 && velocities[i] <= 0) {
            // Линейная интерполяция
            double t_interpolated = times[i-1] + (0 - velocities[i-1]) * (times[i] - times[i-1]) / (velocities[i] - velocities[i-1]);
            std::cout << "Время остановки по данным (интерполяция): " << t_interpolated << " с" << std::endl;
            std::cout << "Разница с теоретическим: " << fabs(t_interpolated - t_zero) << " с" << std::endl;
            break;
        }
    }
    
    return 0;
}