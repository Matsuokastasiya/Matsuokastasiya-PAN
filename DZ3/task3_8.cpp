#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <random>
#include <string>  


// Структура для хранения информации о траектории ракеты
struct RocketTrajectory {
    std::string name;
    std::vector<double> times;
    std::vector<double> heights;
    
    double max_height;
    double time_at_max_height;
    double fall_time;
    bool has_fallen;
    
    RocketTrajectory(const std::string& n, 
                     const std::vector<double>& t, 
                     const std::vector<double>& h)
        : name(n), times(t), heights(h) {
        calculateMetrics();
    }
    
private:
    void calculateMetrics() {
        // Находим максимальную высоту
        if (heights.empty()) {
            max_height = 0.0;
            time_at_max_height = 0.0;
            fall_time = 0.0;
            has_fallen = false;
            return;
        }
        
        // Максимальная высота и время ее достижения
        auto max_iter = std::max_element(heights.begin(), heights.end());
        size_t max_idx = std::distance(heights.begin(), max_iter);
        max_height = *max_iter;
        time_at_max_height = times[max_idx];
        
        // Момент падения (первая высота <= 0)
        auto fall_iter = std::find_if(heights.begin(), heights.end(),
                                     [](double h) { return h <= 0.0; });
        
        if (fall_iter != heights.end()) {
            size_t fall_idx = std::distance(heights.begin(), fall_iter);
            fall_time = times[fall_idx];
            has_fallen = true;
            
            // Если упали сразу (в начальный момент)
            if (fall_idx == 0) {
                max_height = 0.0;
                time_at_max_height = 0.0;
            }
        } else {
            // Не упали в течение наблюдения
            fall_time = times.back();  // последнее время наблюдения
            has_fallen = false;
        }
    }
};

// Функция для генерации траектории ракеты
std::pair<std::vector<double>, std::vector<double>> generateTrajectory(
    double v0, double g, double dt, double t_max, const std::string& name = "") {
    
    std::vector<double> times;
    std::vector<double> heights;
    
    int estimated_size = static_cast<int>(t_max / dt) + 2;
    times.reserve(estimated_size);
    heights.reserve(estimated_size);
    
    for (double t = 0.0; t <= t_max; t += dt) {
        double h = v0 * t - 0.5 * g * t * t;
        times.push_back(t);
        heights.push_back(h);
        
        // Останавливаемся, если ракета глубоко упала
        if (h < -100.0) {
            break;
        }
    }
    
    return {times, heights};
}

// Функция для вывода информации о траектории
void printTrajectoryInfo(const RocketTrajectory& rocket) {
    std::cout << "\nИнформация о ракете '" << rocket.name << "':" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Количество точек: " << rocket.times.size() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Максимальная высота: " << std::setw(8) << rocket.max_height 
              << " м (при t = " << rocket.time_at_max_height << " с)" << std::endl;
    
    if (rocket.has_fallen) {
        std::cout << "Время падения:      " << std::setw(8) << rocket.fall_time << " с" << std::endl;
        std::cout << "Время полета:       " << std::setw(8) << rocket.fall_time << " с" << std::endl;
    } else {
        std::cout << "Время падения:      не упала в течение наблюдения" << std::endl;
        std::cout << "Время наблюдения:   " << std::setw(8) << rocket.fall_time << " с" << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
}

// Функция для сравнения двух ракет
void compareRockets(const RocketTrajectory& r1, const RocketTrajectory& r2) {
    std::cout << "\n\n=== СРАВНЕНИЕ РАКЕТ ===" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    // Таблица сравнения
    std::cout << std::setw(30) << "Параметр" 
              << std::setw(25) << r1.name 
              << std::setw(25) << r2.name << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    std::cout << std::fixed << std::setprecision(2)
              << std::setw(30) << "Максимальная высота (м)"
              << std::setw(25) << r1.max_height
              << std::setw(25) << r2.max_height << std::endl;
    
    std::cout << std::setw(30) << "Время макс. высоты (с)"
              << std::setw(25) << r1.time_at_max_height
              << std::setw(25) << r2.time_at_max_height << std::endl;
    
    if (r1.has_fallen && r2.has_fallen) {
        std::cout << std::setw(30) << "Время падения (с)"
                  << std::setw(25) << r1.fall_time
                  << std::setw(25) << r2.fall_time << std::endl;
    } else {
        std::cout << std::setw(30) << "Состояние"
                  << std::setw(25) << (r1.has_fallen ? "упала" : "в полете")
                  << std::setw(25) << (r2.has_fallen ? "упала" : "в полете") << std::endl;
        
        std::cout << std::setw(30) << "Последнее время (с)"
                  << std::setw(25) << r1.fall_time
                  << std::setw(25) << r2.fall_time << std::endl;
    }
    std::cout << std::string(80, '-') << std::endl;
    
    // Сравнение максимальных высот
    std::cout << "\n1. СРАВНЕНИЕ ВЫСОТЫ:" << std::endl;
    
    if (r1.max_height > r2.max_height) {
        std::cout << "✓ Ракета '" << r1.name << "' поднялась ВЫШЕ" << std::endl;
        std::cout << "  Превышение: " << (r1.max_height - r2.max_height) << " м" << std::endl;
        std::cout << "  Относительное преимущество: " 
                  << ((r1.max_height - r2.max_height) / r2.max_height * 100) << "%" << std::endl;
    } else if (r2.max_height > r1.max_height) {
        std::cout << "✓ Ракета '" << r2.name << "' поднялась ВЫШЕ" << std::endl;
        std::cout << "  Превышение: " << (r2.max_height - r1.max_height) << " м" << std::endl;
        std::cout << "  Относительное преимущество: " 
                  << ((r2.max_height - r1.max_height) / r1.max_height * 100) << "%" << std::endl;
    } else {
        std::cout << "≡ Обе ракеты поднялись на ОДИНАКОВУЮ высоту" << std::endl;
    }
    
    // Сравнение времени полета
    std::cout << "\n2. СРАВНЕНИЕ ВРЕМЕНИ ПОЛЕТА:" << std::endl;
    
    double flight_time1 = r1.fall_time;  // время до падения или наблюдения
    double flight_time2 = r2.fall_time;
    
    // Правила сравнения:
    // 1. Если обе упали - сравниваем времена падения
    // 2. Если одна упала, а другая нет - та, что не упала, считается дольше в воздухе
    // 3. Если обе не упали - сравниваем последние времена наблюдения
    
    if (r1.has_fallen && r2.has_fallen) {
        // Обе упали
        if (flight_time1 > flight_time2) {
            std::cout << "✓ Ракета '" << r1.name << "' дольше находилась В ВОЗДУХЕ" << std::endl;
            std::cout << "  Разница: " << (flight_time1 - flight_time2) << " с" << std::endl;
        } else if (flight_time2 > flight_time1) {
            std::cout << "✓ Ракета '" << r2.name << "' дольше находилась В ВОЗДУХЕ" << std::endl;
            std::cout << "  Разница: " << (flight_time2 - flight_time1) << " с" << std::endl;
        } else {
            std::cout << "≡ Обе ракеты находились в воздухе ОДИНАКОВОЕ время" << std::endl;
        }
    } else if (!r1.has_fallen && !r2.has_fallen) {
        // Обе не упали
        if (flight_time1 > flight_time2) {
            std::cout << "✓ Ракета '" << r1.name << "' дольше наблюдалась (обе еще в полете)" << std::endl;
        } else if (flight_time2 > flight_time1) {
            std::cout << "✓ Ракета '" << r2.name << "' дольше наблюдалась (обе еще в полете)" << std::endl;
        } else {
            std::cout << "≡ Обе ракеты наблюдались одинаковое время (обе еще в полете)" << std::endl;
        }
    } else {
        // Одна упала, другая нет
        if (!r1.has_fallen) {
            std::cout << "✓ Ракета '" << r1.name << "' дольше в воздухе (все еще летит)" << std::endl;
            std::cout << "  '" << r2.name << "' уже упала через " << flight_time2 << " с" << std::endl;
        } else {
            std::cout << "✓ Ракета '" << r2.name << "' дольше в воздухе (все еще летит)" << std::endl;
            std::cout << "  '" << r1.name << "' уже упала через " << flight_time1 << " с" << std::endl;
        }
    }
    
    std::cout << std::string(80, '=') << std::endl;
}

// Функция для вывода траектории (первые несколько точек)
void printTrajectorySample(const RocketTrajectory& rocket, int num_points = 5) {
    std::cout << "\nПервые " << num_points << " точек траектории '" << rocket.name << "':" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(10) << "Индекс" 
              << std::setw(15) << "Время (с)" 
              << std::setw(20) << "Высота (м)" 
              << std::setw(15) << "Состояние" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    size_t points_to_show = std::min((size_t)num_points, rocket.times.size());
    
    for (size_t i = 0; i < points_to_show; ++i) {
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << i
                  << std::setw(15) << rocket.times[i]
                  << std::setw(20) << rocket.heights[i];
        
        if (rocket.heights[i] <= 0.0) {
            std::cout << std::setw(15) << "на земле";
        } else if (i == 0) {
            std::cout << std::setw(15) << "старт";
        } else {
            std::cout << std::setw(15) << "в полете";
        }
        std::cout << std::endl;
    }
    
    if (rocket.times.size() > points_to_show) {
        std::cout << "..." << std::endl;
        
        // Показываем последние точки
        size_t start = rocket.times.size() - 3;
        if (start < points_to_show) start = points_to_show;
        
        for (size_t i = start; i < rocket.times.size(); ++i) {
            std::cout << std::fixed << std::setprecision(3)
                      << std::setw(10) << i
                      << std::setw(15) << rocket.times[i]
                      << std::setw(20) << rocket.heights[i];
            
            if (rocket.heights[i] <= 0.0) {
                std::cout << std::setw(15) << "на земле";
            } else {
                std::cout << std::setw(15) << "в полете";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::string(60, '-') << std::endl;
}

int main() {
    std::cout << "=== ЗАДАЧА 8: СРАВНЕНИЕ ТРАЕКТОРИЙ ===" << std::endl;
    
    // Тест 1: Две разные ракеты
    std::cout << "\n\n=== ТЕСТ 1: Две разные ракеты ===" << std::endl;
    
    // Ракета 1: Мощная, но тяжелая
    auto [times1, heights1] = generateTrajectory(80.0, 9.8, 0.2, 20.0);
    RocketTrajectory rocket1("Атлас-V", times1, heights1);
    
    // Ракета 2: Легкая и быстрая
    auto [times2, heights2] = generateTrajectory(100.0, 9.8, 0.2, 20.0);
    RocketTrajectory rocket2("Фалькон-9", times2, heights2);
    
    printTrajectoryInfo(rocket1);
    printTrajectoryInfo(rocket2);
    compareRockets(rocket1, rocket2);
    
    // Тест 2: Ракета, которая не успела упасть
    std::cout << "\n\n=== ТЕСТ 2: Ракета, которая не успела упасть ===" << std::endl;
    
    // Ракета 3: Успевает упасть
    auto [times3, heights3] = generateTrajectory(40.0, 9.8, 0.2, 15.0);
    RocketTrajectory rocket3("Союз", times3, heights3);
    
    // Ракета 4: Не успевает упасть за время наблюдения
    auto [times4, heights4] = generateTrajectory(40.0, 9.8, 0.2, 5.0);  // мало времени наблюдения
    RocketTrajectory rocket4("Протон-М", times4, heights4);
    
    printTrajectoryInfo(rocket3);
    printTrajectoryInfo(rocket4);
    compareRockets(rocket3, rocket4);
    
    // Тест 3: Ракета, которая сразу падает
    std::cout << "\n\n=== ТЕСТ 3: Ракета, которая сразу падает ===" << std::endl;
    
    // Ракета 5: Сразу падает (начальная скорость 0)
    auto [times5, heights5] = generateTrajectory(0.0, 9.8, 0.2, 10.0);
    RocketTrajectory rocket5("Спутник-1", times5, heights5);
    
    // Ракета 6: Нормальный полет
    auto [times6, heights6] = generateTrajectory(50.0, 9.8, 0.2, 10.0);
    RocketTrajectory rocket6("Восток-1", times6, heights6);
    
    printTrajectoryInfo(rocket5);
    printTrajectoryInfo(rocket6);
    compareRockets(rocket5, rocket6);
    
    // Тест 4: Несколько ракет для турнира
    std::cout << "\n\n=== ТЕСТ 4: Турнир 4-х ракет ===" << std::endl;
    
    std::vector<RocketTrajectory> rockets;
    
    rockets.emplace_back("Ангара", 
        generateTrajectory(60.0, 9.8, 0.2, 15.0).first,
        generateTrajectory(60.0, 9.8, 0.2, 15.0).second);
    
    rockets.emplace_back("Дельта-IV", 
        generateTrajectory(70.0, 9.8, 0.2, 12.0).first,
        generateTrajectory(70.0, 9.8, 0.2, 12.0).second);
    
    rockets.emplace_back("Ариан-5", 
        generateTrajectory(90.0, 9.8, 0.2, 18.0).first,
        generateTrajectory(90.0, 9.8, 0.2, 18.0).second);
    
    rockets.emplace_back("Чанчжэн", 
        generateTrajectory(55.0, 9.8, 0.2, 20.0).first,
        generateTrajectory(55.0, 9.8, 0.2, 20.0).second);
    
    // Вывод информации о всех ракетах
    std::cout << "\nСводная информация о всех ракетах:" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    std::cout << std::setw(15) << "Ракета"
              << std::setw(20) << "Макс. высота (м)"
              << std::setw(20) << "Время макс. (с)"
              << std::setw(20) << "Время полета (с)"
              << std::setw(25) << "Состояние" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    for (const auto& rocket : rockets) {
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(15) << rocket.name
                  << std::setw(20) << rocket.max_height
                  << std::setw(20) << rocket.time_at_max_height
                  << std::setw(20) << rocket.fall_time
                  << std::setw(25) << (rocket.has_fallen ? "упала" : "в полете") << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;
    
    // Определение победителей
    std::cout << "\nРЕЗУЛЬТАТЫ ТУРНИРА:" << std::endl;
    
    // Победитель по высоте
    auto winner_height = std::max_element(rockets.begin(), rockets.end(),
        [](const RocketTrajectory& a, const RocketTrajectory& b) {
            return a.max_height < b.max_height;
        });
    
    std::cout << "\n🏆 ПОБЕДИТЕЛЬ ПО ВЫСОТЕ: '" << winner_height->name << "'" << std::endl;
    std::cout << "   Высота: " << winner_height->max_height << " м" << std::endl;
    
    // Победитель по времени полета
    auto winner_time = std::max_element(rockets.begin(), rockets.end(),
        [](const RocketTrajectory& a, const RocketTrajectory& b) {
            // Учитываем, что ракеты, которые не упали, имеют преимущество
            if (!a.has_fallen && b.has_fallen) return false;
            if (a.has_fallen && !b.has_fallen) return true;
            return a.fall_time < b.fall_time;
        });
    
    std::cout << "\n⏱️ ПОБЕДИТЕЛЬ ПО ВРЕМЕНИ ПОЛЕТА: '" << winner_time->name << "'" << std::endl;
    std::cout << "   Время: " << winner_time->fall_time << " с";
    if (!winner_time->has_fallen) {
        std::cout << " (все еще в полете!)";
    }
    std::cout << std::endl;
    
    // Тест 5: Траектории с разными параметрами дискретизации
    std::cout << "\n\n=== ТЕСТ 5: Влияние дискретизации на сравнение ===" << std::endl;
    
    // Одна и та же ракета, но с разными dt
    std::cout << "\nСравнение одной ракеты с разным шагом времени dt:" << std::endl;
    
    double v0_test = 50.0, g_test = 9.8, t_max_test = 15.0;
    
    std::vector<double> dts = {1.0, 0.5, 0.2, 0.1, 0.05};
    std::vector<RocketTrajectory> same_rocket_different_dt;
    
    for (size_t i = 0; i < dts.size(); ++i) {
        auto [t, h] = generateTrajectory(v0_test, g_test, dts[i], t_max_test);
        std::string name = "Ракета-dt=" + std::to_string(dts[i]);
        same_rocket_different_dt.emplace_back(name, t, h);
    }
    
    // Таблица сравнения
    std::cout << "\nВлияние dt на результаты:" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    std::cout << std::setw(15) << "dt (с)"
              << std::setw(15) << "Точек"
              << std::setw(20) << "Макс. высота (м)"
              << std::setw(20) << "Время макс. (с)"
              << std::setw(20) << "Время падения (с)" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    
    double theoretical_max_h = (v0_test * v0_test) / (2 * g_test);
    double theoretical_t_max = v0_test / g_test;
    double theoretical_t_fall = 2 * v0_test / g_test;
    
    for (const auto& rocket : same_rocket_different_dt) {
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(15) << rocket.name.substr(rocket.name.find('=') + 1)
                  << std::setw(15) << rocket.times.size()
                  << std::setw(20) << rocket.max_height
                  << std::setw(20) << rocket.time_at_max_height
                  << std::setw(20) << rocket.fall_time << std::endl;
    }
    std::cout << std::string(90, '-') << std::endl;
    std::cout << std::setw(15) << "Теория"
              << std::setw(15) << "-"
              << std::setw(20) << theoretical_max_h
              << std::setw(20) << theoretical_t_max
              << std::setw(20) << theoretical_t_fall << std::endl;
    
    // Тест 6: Реальные данные (пример)
    std::cout << "\n\n=== ТЕСТ 6: Пример с реальными данными ===" << std::endl;
    
    // Данные измерений высоты для двух ракет
    std::vector<double> real_times = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
    
    std::vector<double> real_heights1 = {0.0, 45.1, 80.4, 105.9, 121.6, 127.5, 123.6, 109.9, 86.4, 53.1};
    std::vector<double> real_heights2 = {0.0, 48.2, 86.7, 115.4, 134.3, 143.4, 142.7, 132.2, 111.9, 81.8};
    
    RocketTrajectory real_rocket1("Эксперимент-А", real_times, real_heights1);
    RocketTrajectory real_rocket2("Эксперимент-Б", real_times, real_heights2);
    
    // Добавляем еще точки для второй ракеты (она летит дольше)
    real_heights2.push_back(41.9);  // t=10.0
    real_heights2.push_back(-7.8);  // t=11.0 - упала
    real_times.push_back(10.0);
    real_times.push_back(11.0);
    
    // Обновляем данные второй ракеты
    real_rocket2 = RocketTrajectory("Эксперимент-Б", real_times, real_heights2);
    
    printTrajectorySample(real_rocket1);
    printTrajectorySample(real_rocket2);
    compareRockets(real_rocket1, real_rocket2);
    
    // Заключение
    std::cout << "\n\n=== ВЫВОДЫ И РЕКОМЕНДАЦИИ ===" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "1. Для сравнения траекторий используйте:" << std::endl;
    std::cout << "   - std::max_element для поиска максимальной высоты" << std::endl;
    std::cout << "   - std::find_if для определения момента падения" << std::endl;
    std::cout << "   - std::distance для получения индексов" << std::endl;
    std::cout << "\n2. Важные моменты при сравнении:" << std::endl;
    std::cout << "   - Учитывайте, если ракета не упала за время наблюдения" << std::endl;
    std::cout << "   - Проверяйте случай падения в начальный момент" << std::endl;
    std::cout << "   - Убедитесь, что временные интервалы одинаковы" << std::endl;
    std::cout << "\n3. Точность сравнения зависит от:" << std::endl;
    std::cout << "   - Шага дискретизации dt" << std::endl;
    std::cout << "   - Длительности наблюдения" << std::endl;
    std::cout << "   - Качества измерений" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}