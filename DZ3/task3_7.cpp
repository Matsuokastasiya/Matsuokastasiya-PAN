#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <random>

// Функция для расчета траектории
std::pair<std::vector<double>, std::vector<double>> calculateTrajectory(
    double v0, double g, double dt, double t_max) {
    
    std::vector<double> times;
    std::vector<double> heights;
    
    int estimated_size = static_cast<int>(t_max / dt) + 2;
    times.reserve(estimated_size);
    heights.reserve(estimated_size);
    
    for (double t = 0.0; t <= t_max; t += dt) {
        double h = v0 * t - 0.5 * g * t * t;
        
        // Останавливаемся, если высота сильно отрицательная
        if (h < -100.0) {
            break;
        }
        
        times.push_back(t);
        heights.push_back(h);
    }
    
    return {times, heights};
}

// Функция для поиска момента падения
std::pair<bool, size_t> findFallingPoint(const std::vector<double>& heights) {
    if (heights.empty()) {
        return {false, 0};
    }
    
    // Проверяем случай, когда начальная высота уже <= 0
    if (heights[0] <= 0.0) {
        return {true, 0};
    }
    
    // Используем std::find_if для поиска первого элемента <= 0
    auto it = std::find_if(heights.begin(), heights.end(),
                          [](double h) { return h <= 0.0; });
    
    if (it != heights.end()) {
        size_t idx = std::distance(heights.begin(), it);
        return {true, idx};
    }
    
    return {false, 0};
}

// Функция для вывода траектории
void printTrajectory(const std::vector<double>& times, 
                     const std::vector<double>& heights,
                     const std::string& title,
                     size_t highlight_idx = -1) {
    
    std::cout << title << " (" << times.size() << " точек):" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::setw(10) << "Индекс" 
              << std::setw(15) << "Время (с)" 
              << std::setw(20) << "Высота (м)" 
              << std::setw(30) << "Примечание" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    size_t points_to_show = std::min((size_t)15, times.size());
    
    for (size_t i = 0; i < points_to_show; ++i) {
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << i
                  << std::setw(15) << times[i]
                  << std::setw(20) << heights[i];
        
        if (i == highlight_idx) {
            std::cout << std::setw(30) << "← МОМЕНТ ПАДЕНИЯ";
        } else if (heights[i] <= 0.0) {
            std::cout << std::setw(30) << "высота <= 0";
        } else if (i < highlight_idx) {
            std::cout << std::setw(30) << "в полете";
        }
        std::cout << std::endl;
    }
    
    if (times.size() > points_to_show) {
        std::cout << "..." << std::endl;
        
        // Показываем последние точки
        size_t start = times.size() - 3;
        if (start < points_to_show) start = points_to_show;
        
        for (size_t i = start; i < times.size(); ++i) {
            std::cout << std::fixed << std::setprecision(3)
                      << std::setw(10) << i
                      << std::setw(15) << times[i]
                      << std::setw(20) << heights[i];
            
            if (i == highlight_idx) {
                std::cout << std::setw(30) << "← МОМЕНТ ПАДЕНИЯ";
            } else if (heights[i] <= 0.0) {
                std::cout << std::setw(30) << "высота <= 0";
            }
            std::cout << std::endl;
        }
    }
    
    std::cout << std::string(80, '-') << std::endl;
}

int main() {
    std::cout << "=== ЗАДАЧА 7: НАХОЖДЕНИЕ МОМЕНТА ПАДЕНИЯ ===" << std::endl;
    
    // Параметры для разных тестов
    struct TestCase {
        std::string name;
        double v0;
        double g;
        double dt;
        double t_max;
        bool expected_fall;
    };
    
    std::vector<TestCase> test_cases = {
        {"Нормальный полет", 50.0, 9.8, 0.1, 15.0, true},
        {"Медленный запуск", 10.0, 9.8, 0.1, 5.0, true},
        {"Быстрый запуск", 100.0, 9.8, 0.2, 30.0, true},
        {"Нет падения (мало времени)", 50.0, 9.8, 0.1, 2.0, false},
        {"Старт с земли", 0.0, 9.8, 0.1, 10.0, true},  // падение сразу
        {"Вертикальный спуск", -10.0, 9.8, 0.1, 5.0, true},  // отрицательная начальная скорость
    };
    
    // Тест 1: Основной случай
    std::cout << "\n\n=== ТЕСТ 1: Основной случай (v0 = 50 м/с) ===" << std::endl;
    {
        double v0 = 50.0, g = 9.8, dt = 0.1, t_max = 15.0;
        
        auto [times, heights] = calculateTrajectory(v0, g, dt, t_max);
        
        // Поиск момента падения
        auto [found, idx] = findFallingPoint(heights);
        
        if (found) {
            std::cout << "✓ Падение обнаружено!" << std::endl;
            std::cout << "Индекс падения: " << idx << std::endl;
            std::cout << "Время падения: t = " << times[idx] << " с" << std::endl;
            std::cout << "Высота в момент падения: h = " << heights[idx] << " м" << std::endl;
            
            // Теоретическое время падения
            double theoretical_t_fall = 2.0 * v0 / g;
            std::cout << "Теоретическое время падения: t = 2*v0/g = " 
                      << theoretical_t_fall << " с" << std::endl;
            
            // Разница с теоретическим значением
            std::cout << "Разница: Δt = " << fabs(times[idx] - theoretical_t_fall) 
                      << " с (погрешность дискретизации)" << std::endl;
            
            printTrajectory(times, heights, "Траектория полета", idx);
        } else {
            std::cout << "✗ Падение не обнаружено в диапазоне времени" << std::endl;
            printTrajectory(times, heights, "Траектория полета");
        }
    }
    
    // Тест 2: Все тест-кейсы
    std::cout << "\n\n=== ТЕСТ 2: Все тест-кейсы ===" << std::endl;
    
    std::cout << std::string(100, '-') << std::endl;
    std::cout << std::setw(25) << "Тест"
              << std::setw(10) << "v0"
              << std::setw(10) << "dt"
              << std::setw(15) << "Найдено"
              << std::setw(15) << "Время (с)"
              << std::setw(15) << "Высота (м)"
              << std::setw(20) << "Статус" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    for (const auto& test : test_cases) {
        auto [times, heights] = calculateTrajectory(test.v0, test.g, test.dt, test.t_max);
        auto [found, idx] = findFallingPoint(heights);
        
        std::cout << std::fixed << std::setprecision(1)
                  << std::setw(25) << test.name
                  << std::setw(10) << test.v0
                  << std::setw(10) << test.dt;
        
        if (found) {
            std::cout << std::setw(15) << "ДА"
                      << std::setw(15) << times[idx]
                      << std::setw(15) << heights[idx];
            
            // Проверка корректности
            if (test.expected_fall) {
                std::cout << std::setw(20) << "✓ ОК";
            } else {
                std::cout << std::setw(20) << "✗ Ожидалось: нет";
            }
        } else {
            std::cout << std::setw(15) << "НЕТ"
                      << std::setw(15) << "-"
                      << std::setw(15) << "-";
            
            if (!test.expected_fall) {
                std::cout << std::setw(20) << "✓ ОК";
            } else {
                std::cout << std::setw(20) << "✗ Ожидалось: да";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;
    
    // Тест 3: Особые случаи
    std::cout << "\n\n=== ТЕСТ 3: Особые случаи ===" << std::endl;
    
    // Случай 3.1: Высота никогда не становится <= 0
    std::cout << "\n--- Случай 3.1: Нет падения ---" << std::endl;
    {
        std::vector<double> heights = {10.0, 20.0, 30.0, 25.0, 15.0, 5.0, 1.0};
        std::vector<double> times = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
        
        auto [found, idx] = findFallingPoint(heights);
        
        if (!found) {
            std::cout << "✓ Падение не обнаружено (как и ожидалось)" << std::endl;
            std::cout << "Все высоты положительные" << std::endl;
        }
        
        printTrajectory(times, heights, "Траектория без падения");
    }
    
    // Случай 3.2: Падение в начале
    std::cout << "\n--- Случай 3.2: Падение в начале (t=0) ---" << std::endl;
    {
        std::vector<double> heights = {0.0, -1.0, -5.0, -10.0};
        std::vector<double> times = {0.0, 0.1, 0.2, 0.3};
        
        auto [found, idx] = findFallingPoint(heights);
        
        if (found && idx == 0) {
            std::cout << "✓ Падение обнаружено в начальный момент (t=0)" << std::endl;
            std::cout << "Время падения: t = " << times[idx] << " с" << std::endl;
            std::cout << "Высота: h = " << heights[idx] << " м" << std::endl;
        }
        
        printTrajectory(times, heights, "Траектория с падением в начале", idx);
    }
    
    // Случай 3.3: Несколько точек с высотой <= 0
    std::cout << "\n--- Случай 3.3: Несколько точек с высотой <= 0 ---" << std::endl;
    {
        std::vector<double> heights = {10.0, 5.0, 0.0, -2.0, -5.0, -8.0};
        std::vector<double> times = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0};
        
        auto [found, idx] = findFallingPoint(heights);
        
        if (found) {
            std::cout << "✓ Первое падение обнаружено на индексе " << idx << std::endl;
            std::cout << "Время первого падения: t = " << times[idx] << " с" << std::endl;
            std::cout << "Высота: h = " << heights[idx] << " м" << std::endl;
            
            // Показываем, что после есть еще точки с h <= 0
            std::cout << "\nВсе точки с высотой <= 0:" << std::endl;
            for (size_t i = idx; i < heights.size(); ++i) {
                if (heights[i] <= 0.0) {
                    std::cout << "  t=" << times[i] << " с, h=" << heights[i] << " м" << std::endl;
                }
            }
        }
        
        printTrajectory(times, heights, "Траектория с несколькими падениями", idx);
    }
    
    // Тест 4: Вариант с использованием только dt
    std::cout << "\n\n=== ТЕСТ 4: Вариант с использованием только dt ===" << std::endl;
    {
        double v0 = 50.0, g = 9.8, dt = 0.1, t_max = 15.0;
        
        auto [times, heights] = calculateTrajectory(v0, g, dt, t_max);
        
        // Способ 1: Используя times и heights (основной)
        {
            auto it = std::find_if(heights.begin(), heights.end(),
                                  [](double h) { return h <= 0.0; });
            
            if (it != heights.end()) {
                size_t idx = std::distance(heights.begin(), it);
                std::cout << "Способ 1 (с times):" << std::endl;
                std::cout << "  Время падения: t = " << times[idx] << " с" << std::endl;
            }
        }
        
        // Способ 2: Используя только dt
        {
            auto it = std::find_if(heights.begin(), heights.end(),
                                  [](double h) { return h <= 0.0; });
            
            if (it != heights.end()) {
                size_t idx = std::distance(heights.begin(), it);
                double t_fall_dt = idx * dt;
                std::cout << "\nСпособ 2 (только dt):" << std::endl;
                std::cout << "  Время падения: t = idx * dt = " << idx << " * " << dt 
                          << " = " << t_fall_dt << " с" << std::endl;
                
                // Сравнение с первым способом
                double diff = fabs(t_fall_dt - times[idx]);
                std::cout << "  Разница со способом 1: " << diff << " с" << std::endl;
                
                if (diff < 1e-10) {
                    std::cout << "  ✓ Оба способа дают одинаковый результат" << std::endl;
                }
            }
        }
        
        // Способ 3: Линейная интерполяция для большей точности
        {
            auto it = std::find_if(heights.begin(), heights.end(),
                                  [](double h) { return h <= 0.0; });
            
            if (it != heights.end() && it != heights.begin()) {
                size_t idx = std::distance(heights.begin(), it);
                
                // Линейная интерполяция между точками
                double h1 = heights[idx-1];  // последняя положительная высота
                double h2 = heights[idx];    // первая неположительная высота
                double t1 = times[idx-1];
                double t2 = times[idx];
                
                // Интерполяция: h = h1 + (h2-h1)*(t-t1)/(t2-t1)
                // Решаем уравнение h = 0 относительно t
                double t_interpolated = t1 - h1 * (t2 - t1) / (h2 - h1);
                
                std::cout << "\nСпособ 3 (линейная интерполяция):" << std::endl;
                std::cout << "  Более точное время падения: t = " << t_interpolated << " с" << std::endl;
                
                // Теоретическое значение
                double theoretical = 2.0 * v0 / g;
                std::cout << "  Теоретическое значение: t = 2*v0/g = " << theoretical << " с" << std::endl;
                std::cout << "  Погрешность: " << fabs(t_interpolated - theoretical) << " с" << std::endl;
            }
        }
    }
    
    // Тест 5: Граничные условия и точность
    std::cout << "\n\n=== ТЕСТ 5: Влияние шага времени dt на точность ===" << std::endl;
    
    double v0 = 50.0, g = 9.8;
    double theoretical_t_fall = 2.0 * v0 / g;
    
    std::cout << "Теоретическое время падения: t = 2*v0/g = " << theoretical_t_fall << " с" << std::endl;
    std::cout << "\nЗависимость точности от шага времени dt:" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::setw(15) << "dt (с)"
              << std::setw(15) << "Найденное t"
              << std::setw(15) << "Погрешность"
              << std::setw(15) << "Индекс"
              << std::setw(20) << "Высота в момент t" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    std::vector<double> dts = {1.0, 0.5, 0.2, 0.1, 0.05, 0.01, 0.005};
    
    for (double dt : dts) {
        double t_max = theoretical_t_fall * 1.5;  // чтобы точно захватить момент падения
        
        auto [times, heights] = calculateTrajectory(v0, g, dt, t_max);
        auto [found, idx] = findFallingPoint(heights);
        
        if (found) {
            double found_time = times[idx];
            double error = fabs(found_time - theoretical_t_fall);
            
            std::cout << std::fixed << std::setprecision(3)
                      << std::setw(15) << dt
                      << std::setw(15) << found_time
                      << std::setw(15) << error
                      << std::setw(15) << idx
                      << std::setw(20) << heights[idx] << std::endl;
        } else {
            std::cout << std::fixed << std::setprecision(3)
                      << std::setw(15) << dt
                      << std::setw(15) << "не найден"
                      << std::setw(15) << "-"
                      << std::setw(15) << "-"
                      << std::setw(20) << "-" << std::endl;
        }
    }
    std::cout << std::string(80, '-') << std::endl;
    
    // Тест 6: Практическое применение
    std::cout << "\n\n=== ТЕСТ 6: Практическое применение ===" << std::endl;
    
    // Симуляция полета с помехами
    std::cout << "Симуляция полета с помехами в измерениях:" << std::endl;
    
    std::mt19937 gen(12345);
    std::normal_distribution<double> noise(0.0, 0.5);
    
    double v0_sim = 30.0, g_sim = 9.8, dt_sim = 0.2;
    double t_fall_theoretical = 2.0 * v0_sim / g_sim;
    
    std::vector<double> times_sim;
    std::vector<double> heights_sim;
    
    for (double t = 0.0; t <= t_fall_theoretical * 1.5; t += dt_sim) {
        double h_clean = v0_sim * t - 0.5 * g_sim * t * t;
        double h_noisy = h_clean + noise(gen);
        
        times_sim.push_back(t);
        heights_sim.push_back(h_noisy);
    }
    
    // Поиск момента падения в зашумленных данных
    auto it_fall = std::find_if(heights_sim.begin(), heights_sim.end(),
                               [](double h) { return h <= 0.0; });
    
    if (it_fall != heights_sim.end()) {
        size_t idx_fall = std::distance(heights_sim.begin(), it_fall);
        
        std::cout << "\nРезультаты:" << std::endl;
        std::cout << "Теоретическое время падения: " << t_fall_theoretical << " с" << std::endl;
        std::cout << "Найденное время падения: " << times_sim[idx_fall] << " с" << std::endl;
        std::cout << "Разница: " << fabs(times_sim[idx_fall] - t_fall_theoretical) << " с" << std::endl;
        
        // Показываем окрестности момента падения
        std::cout << "\nОкрестности момента падения:" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        int window = 3;
        size_t start = (idx_fall > window) ? idx_fall - window : 0;
        size_t end = std::min(heights_sim.size(), idx_fall + window + 1);
        
        for (size_t i = start; i < end; ++i) {
            std::cout << std::fixed << std::setprecision(3)
                      << "t=" << std::setw(6) << times_sim[i] << " с: "
                      << std::setw(8) << heights_sim[i] << " м";
            
            if (i == idx_fall) {
                std::cout << "  ← ПАДЕНИЕ (первое h <= 0)";
            } else if (heights_sim[i] <= 0.0) {
                std::cout << "  (h <= 0)";
            }
            std::cout << std::endl;
        }
        std::cout << std::string(60, '-') << std::endl;
    }
    
    std::cout << "\n=== ВЫВОДЫ ===" << std::endl;
    std::cout << "1. std::find_if эффективно находит первый элемент, удовлетворяющий условию" << std::endl;
    std::cout << "2. Важно проверять it == heights.end() для случая, когда падения нет" << std::endl;
    std::cout << "3. Можно использовать как times[idx], так и idx * dt" << std::endl;
    std::cout << "4. Для большей точности можно использовать линейную интерполяцию" << std::endl;
    std::cout << "5. Меньший шаг dt увеличивает точность определения момента падения" << std::endl;
    
    return 0;
}