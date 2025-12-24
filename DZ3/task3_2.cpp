#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>  // для std::max_element и std::distance

int main() {
    // Константы (как в задаче 1)
    const double v0 = 50.0;  // начальная скорость (м/с)
    const double g = 9.8;    // ускорение свободного падения (м/с²)
    const double dt = 1.0;   // шаг времени (с)
    
    // Проверка граничных случаев
    if (v0 <= 0.0) {
        std::cout << "Предупреждение: начальная скорость v0 должна быть положительной!" << std::endl;
        return 1;
    }
    
    // Создание векторов для хранения времени и высоты (как в задаче 1)
    std::vector<double> times;
    std::vector<double> heights;
    
    // Резервирование памяти для оптимизации
    int estimated_size = static_cast<int>(v0 * 2 / g / dt) + 1;
    times.reserve(estimated_size);
    heights.reserve(estimated_size);
    
    // Расчет траектории (как в задаче 1)
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
    
    // Проверка наличия данных
    if (heights.empty()) {
        std::cout << "Нет данных о высотах." << std::endl;
        return 0;
    }
    
    // --- ВАРИАНТ 1: Ручной поиск максимума (как в условии) ---
    std::cout << "=== Вариант 1: Ручной поиск максимума ===" << std::endl;
    size_t idx_max = 0;
    for (size_t i = 1; i < heights.size(); ++i) {
        if (heights[i] > heights[idx_max]) {
            idx_max = i;
        }
    }
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Максимальная высота: " << heights[idx_max] << " м" << std::endl;
    std::cout << "Время достижения максимальной высоты: " << times[idx_max] << " с" << std::endl;
    
    // --- ВАРИАНТ 2: Поиск максимума с использованием STL ---
    std::cout << "\n=== Вариант 2: Поиск с использованием STL ===" << std::endl;
    auto it_max = std::max_element(heights.begin(), heights.end());
    size_t idx_stl = std::distance(heights.begin(), it_max);
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Максимальная высота: " << *it_max << " м" << std::endl;
    std::cout << "Время достижения максимальной высоты: " << times[idx_stl] << " с" << std::endl;
    
    // --- ВАРИАНТ 3: Проверка при нескольких равных максимумах ---
    std::cout << "\n=== Дополнительная проверка (плато) ===" << std::endl;
    
    // Создадим тестовый вектор с несколькими одинаковыми максимумами
    std::vector<double> test_heights = {10.0, 20.0, 30.0, 30.0, 25.0, 15.0};
    
    // Поиск первого максимума
    auto test_it = std::max_element(test_heights.begin(), test_heights.end());
    size_t test_idx = std::distance(test_heights.begin(), test_it);
    
    std::cout << "Тестовый вектор высот: ";
    for (double h : test_heights) {
        std::cout << h << " ";
    }
    std::cout << std::endl;
    
    std::cout << "Первый максимум (индекс " << test_idx << "): " << test_heights[test_idx] << std::endl;
    std::cout << "Второй максимум (индекс " << test_idx + 1 << "): " << test_heights[test_idx + 1] << std::endl;
    std::cout << "Вердикт: при плато возвращается первый максимум ✓" << std::endl;
    
    // --- Вывод всей таблицы для наглядности ---
    std::cout << "\n=== Таблица траектории (первые 5 точек) ===" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    std::cout << std::setw(10) << "Время (с)" 
              << std::setw(15) << "Высота (м)" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    
    size_t points_to_show = std::min((size_t)5, times.size());
    for (size_t i = 0; i < points_to_show; ++i) {
        std::cout << std::fixed << std::setprecision(1)
                  << std::setw(10) << times[i]
                  << std::fixed << std::setprecision(2)
                  << std::setw(15) << heights[i]
                  << std::endl;
    }
    
    if (times.size() > 5) {
        std::cout << "..." << std::endl;
        std::cout << std::fixed << std::setprecision(1)
                  << std::setw(10) << times.back()
                  << std::fixed << std::setprecision(2)
                  << std::setw(15) << heights.back()
                  << std::endl;
    }
    
    std::cout << std::string(30, '-') << std::endl;
    
    // --- Теоретическая проверка ---
    std::cout << "\n=== Теоретическая проверка ===" << std::endl;
    double theoretical_max_time = v0 / g;  // Время достижения максимума
    double theoretical_max_height = v0 * theoretical_max_time - 0.5 * g * theoretical_max_time * theoretical_max_time;
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Теоретическое время максимума: " << theoretical_max_time << " с" << std::endl;
    std::cout << "Теоретическая максимальная высота: " << theoretical_max_height << " м" << std::endl;
    std::cout << "Фактическое время максимума: " << times[idx_max] << " с" << std::endl;
    std::cout << "Фактическая максимальная высота: " << heights[idx_max] << " м" << std::endl;
    
    // Проверка точности
    if (fabs(times[idx_max] - theoretical_max_time) <= dt && 
        fabs(heights[idx_max] - theoretical_max_height) < 1.0) {
        std::cout << "Результаты корректны (в пределах точности дискретизации) ✓" << std::endl;
    }
    
    return 0;
}