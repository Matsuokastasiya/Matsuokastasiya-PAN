#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <random>
#include <numeric>
#include <functional>
#include <chrono>

// Функция для генерации "чистой" траектории
std::vector<double> generateCleanTrajectory(int n, double v0 = 50.0, double g = 9.8, double dt = 0.1) {
    std::vector<double> trajectory(n);
    for (int i = 0; i < n; ++i) {
        double t = i * dt;
        trajectory[i] = v0 * t - 0.5 * g * t * t;
    }
    return trajectory;
}

// Функция для добавления шума к траектории
std::vector<double> addNoise(const std::vector<double>& clean, 
                            double noise_amplitude, 
                            unsigned int seed = 12345) {
    std::vector<double> noisy = clean;
    std::mt19937 gen(seed);
    std::normal_distribution<double> dist(0.0, noise_amplitude);
    
    for (size_t i = 0; i < noisy.size(); ++i) {
        noisy[i] += dist(gen);
    }
    return noisy;
}

// Функция для расчета скользящего среднего с окном 3
std::vector<double> movingAverage3(const std::vector<double>& data, 
                                  bool handle_borders = true) {
    if (data.empty()) return {};
    
    std::vector<double> smoothed(data.size());
    
    if (data.size() < 3) {
        // Если данных меньше 3, просто копируем
        smoothed = data;
        return smoothed;
    }
    
    // Обработка граничных случаев
    if (handle_borders) {
        // Вариант 1: Оставляем граничные значения как есть
        smoothed[0] = data[0];
        smoothed[data.size() - 1] = data[data.size() - 1];
        
        // Основной цикл
        for (size_t i = 1; i + 1 < data.size(); ++i) {
            smoothed[i] = (data[i-1] + data[i] + data[i+1]) / 3.0;
        }
    } else {
        // Вариант 2: Используем меньшее окно для границ
        // Первый элемент: среднее из первых двух
        smoothed[0] = (data[0] + data[1]) / 2.0;
        
        // Последний элемент: среднее из последних двух
        smoothed[data.size() - 1] = (data[data.size() - 2] + data[data.size() - 1]) / 2.0;
        
        // Основной цикл
        for (size_t i = 1; i + 1 < data.size(); ++i) {
            smoothed[i] = (data[i-1] + data[i] + data[i+1]) / 3.0;
        }
    }
    
    return smoothed;
}

// Функция для расчета скользящего среднего с произвольным окном (2k+1)
std::vector<double> movingAverage(const std::vector<double>& data, 
                                 int k, 
                                 bool use_prefix_sum = false) {
    if (data.empty() || k <= 0) return {};
    
    int window_size = 2 * k + 1;
    std::vector<double> smoothed(data.size());
    
    if (window_size > static_cast<int>(data.size())) {
        // Окно больше размера данных
        std::fill(smoothed.begin(), smoothed.end(), 
                 std::accumulate(data.begin(), data.end(), 0.0) / data.size());
        return smoothed;
    }
    
    if (!use_prefix_sum) {
        // Простой метод: прямое суммирование
        for (int i = 0; i < static_cast<int>(data.size()); ++i) {
            int start = std::max(0, i - k);
            int end = std::min(static_cast<int>(data.size()) - 1, i + k);
            int count = end - start + 1;
            
            double sum = 0.0;
            for (int j = start; j <= end; ++j) {
                sum += data[j];
            }
            smoothed[i] = sum / count;
        }
    } else {
        // Эффективный метод: префиксные суммы
        std::vector<double> prefix_sum(data.size() + 1, 0.0);
        for (size_t i = 0; i < data.size(); ++i) {
            prefix_sum[i + 1] = prefix_sum[i] + data[i];
        }
        
        for (int i = 0; i < static_cast<int>(data.size()); ++i) {
            int start = std::max(0, i - k);
            int end = std::min(static_cast<int>(data.size()) - 1, i + k);
            int count = end - start + 1;
            
            smoothed[i] = (prefix_sum[end + 1] - prefix_sum[start]) / count;
        }
    }
    
    return smoothed;
}

// Функция для расчета среднеквадратичной ошибки (MSE)
double calculateMSE(const std::vector<double>& original, 
                   const std::vector<double>& smoothed) {
    if (original.size() != smoothed.size() || original.empty()) {
        return 0.0;
    }
    
    double sum_squared_error = 0.0;
    for (size_t i = 0; i < original.size(); ++i) {
        double error = original[i] - smoothed[i];
        sum_squared_error += error * error;
    }
    
    return sum_squared_error / original.size();
}

// Функция для вывода двух векторов в виде колонок
void printVectorsSideBySide(const std::vector<double>& vec1, 
                           const std::vector<double>& vec2,
                           const std::string& name1,
                           const std::string& name2,
                           int max_rows = 20) {
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::setw(10) << "Индекс" 
              << std::setw(25) << name1 
              << std::setw(25) << name2 
              << std::setw(20) << "Разница" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    size_t rows_to_show = std::min(static_cast<size_t>(max_rows), vec1.size());
    
    for (size_t i = 0; i < rows_to_show; ++i) {
        double diff = vec1[i] - vec2[i];
        
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << i
                  << std::setw(25) << vec1[i]
                  << std::setw(25) << vec2[i]
                  << std::setw(20) << diff << std::endl;
    }
    
    if (vec1.size() > rows_to_show) {
        std::cout << "..." << std::endl;
        
        // Показываем последние несколько строк
        size_t start = vec1.size() - 3;
        if (start < rows_to_show) start = rows_to_show;
        
        for (size_t i = start; i < vec1.size(); ++i) {
            double diff = vec1[i] - vec2[i];
            
            std::cout << std::fixed << std::setprecision(3)
                      << std::setw(10) << i
                      << std::setw(25) << vec1[i]
                      << std::setw(25) << vec2[i]
                      << std::setw(20) << diff << std::endl;
        }
    }
    
    std::cout << std::string(80, '-') << std::endl;
}

// Функция для визуализации вектора (текстовый график)
void visualizeVector(const std::vector<double>& data, 
                    const std::string& title,
                    double y_min, double y_max,
                    int height = 20, int width = 60) {
    
    std::cout << "\n" << title << ":" << std::endl;
    std::cout << "Диапазон значений: [" << y_min << ", " << y_max << "]" << std::endl;
    
    // Создаем матрицу для отрисовки
    std::vector<std::vector<char>> canvas(height, std::vector<char>(width, ' '));
    
    // Масштабируем данные для отображения
    double y_scale = (y_max - y_min) / (height - 1);
    double x_scale = static_cast<double>(data.size() - 1) / (width - 1);
    
    // Рисуем ось Y
    for (int row = 0; row < height; ++row) {
        double y_value = y_max - row * y_scale;
        if (row % 2 == 0) {
            std::cout << std::setw(8) << std::fixed << std::setprecision(1) << y_value << " |";
        } else {
            std::cout << "         |";
        }
        
        // Рисуем данные
        for (int col = 0; col < width; ++col) {
            double x_index = col * x_scale;
            int idx1 = static_cast<int>(x_index);
            int idx2 = std::min(idx1 + 1, static_cast<int>(data.size()) - 1);
            double t = x_index - idx1;
            
            double y_data;
            if (idx1 == idx2) {
                y_data = data[idx1];
            } else {
                y_data = data[idx1] * (1 - t) + data[idx2] * t;
            }
            
            int plot_row = static_cast<int>((y_max - y_data) / y_scale);
            
            if (plot_row >= 0 && plot_row < height) {
                if (plot_row == row) {
                    std::cout << "*";
                } else {
                    std::cout << " ";
                }
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
    
    // Рисуем ось X
    std::cout << "         ";
    for (int i = 0; i <= width; ++i) {
        std::cout << "-";
    }
    std::cout << std::endl;
    
    std::cout << "         ";
    for (int col = 0; col < width; col += 10) {
        std::cout << std::setw(10) << col * data.size() / width;
    }
    std::cout << " (индекс)" << std::endl;
}

int main() {
    std::cout << "=== ЗАДАЧА 9: СГЛАЖИВАНИЕ ТРАЕКТОРИИ ===" << std::endl;
    
    // Генерация тестовых данных
    std::cout << "\n=== ГЕНЕРАЦИЯ ТЕСТОВЫХ ДАННЫХ ===" << std::endl;
    
    int n_points = 50;
    double dt = 0.1;
    double v0 = 50.0;
    double g = 9.8;
    double noise_amplitude = 5.0;
    
    // Генерируем чистую траекторию
    std::vector<double> clean_trajectory = generateCleanTrajectory(n_points, v0, g, dt);
    
    // Добавляем шум
    std::vector<double> noisy_trajectory = addNoise(clean_trajectory, noise_amplitude);
    
    // Выводим информацию о данных
    std::cout << "Количество точек: " << n_points << std::endl;
    std::cout << "Шаг времени dt: " << dt << " с" << std::endl;
    std::cout << "Амплитуда шума: " << noise_amplitude << " м" << std::endl;
    
    auto [min_it, max_it] = std::minmax_element(clean_trajectory.begin(), clean_trajectory.end());
    std::cout << "Диапазон чистой траектории: [" << *min_it << ", " << *max_it << "] м" << std::endl;
    
    // Тест 1: Скользящее среднее с окном 3
    std::cout << "\n\n=== ТЕСТ 1: Скользящее среднее с окном 3 ===" << std::endl;
    
    // Вариант 1: Граничные значения оставляем как есть
    std::cout << "\n--- Вариант 1: Граничные значения без изменений ---" << std::endl;
    std::vector<double> smoothed1 = movingAverage3(noisy_trajectory, true);
    
    printVectorsSideBySide(noisy_trajectory, smoothed1, 
                          "Зашумленная (м)", "Сглаженная (м)");
    
    double mse1 = calculateMSE(clean_trajectory, smoothed1);
    std::cout << "Среднеквадратичная ошибка (MSE): " << mse1 << std::endl;
    
    // Вариант 2: Граничные значения с меньшим окном
    std::cout << "\n--- Вариант 2: Граничные значения с окном 2 ---" << std::endl;
    std::vector<double> smoothed2 = movingAverage3(noisy_trajectory, false);
    
    printVectorsSideBySide(noisy_trajectory, smoothed2, 
                          "Зашумленная (м)", "Сглаженная (м)");
    
    double mse2 = calculateMSE(clean_trajectory, smoothed2);
    std::cout << "Среднеквадратичная ошибка (MSE): " << mse2 << std::endl;
    
    // Тест 2: Скользящее среднее с разными размерами окна
    std::cout << "\n\n=== ТЕСТ 2: Скользящее среднее с разными размерами окна ===" << std::endl;
    
    std::vector<int> window_sizes = {1, 2, 3, 5, 7, 10};
    
    std::cout << std::string(80, '-') << std::endl;
    std::cout << std::setw(15) << "Размер окна" 
              << std::setw(20) << "Метод" 
              << std::setw(20) << "MSE" 
              << std::setw(25) << "Улучшение (%)" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    // MSE исходных зашумленных данных
    double mse_noisy = calculateMSE(clean_trajectory, noisy_trajectory);
    std::cout << std::setw(15) << "0 (исходный)" 
              << std::setw(20) << "-" 
              << std::setw(20) << mse_noisy 
              << std::setw(25) << "0.0" << std::endl;
    
    for (int k : window_sizes) {
        // Без префиксных сумм
        std::vector<double> smoothed_simple = movingAverage(noisy_trajectory, k, false);
        double mse_simple = calculateMSE(clean_trajectory, smoothed_simple);
        double improvement_simple = (mse_noisy - mse_simple) / mse_noisy * 100;
        
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(15) << (2*k + 1)
                  << std::setw(20) << "простой"
                  << std::setw(20) << mse_simple
                  << std::setw(25) << improvement_simple << std::endl;
        
        // С префиксными суммами
        std::vector<double> smoothed_prefix = movingAverage(noisy_trajectory, k, true);
        double mse_prefix = calculateMSE(clean_trajectory, smoothed_prefix);
        double improvement_prefix = (mse_noisy - mse_prefix) / mse_noisy * 100;
        
        std::cout << std::setw(15) << (2*k + 1)
                  << std::setw(20) << "префиксный"
                  << std::setw(20) << mse_prefix
                  << std::setw(25) << improvement_prefix << std::endl;
    }
    std::cout << std::string(80, '-') << std::endl;
    
    // Тест 3: Визуализация результатов
    std::cout << "\n\n=== ТЕСТ 3: ВИЗУАЛИЗАЦИЯ РЕЗУЛЬТАТОВ ===" << std::endl;
    
    // Находим общий диапазон для масштабирования графиков
    double overall_min = *std::min_element(clean_trajectory.begin(), clean_trajectory.end());
    double overall_max = *std::max_element(clean_trajectory.begin(), clean_trajectory.end());
    
    // Добавляем немного места для отображения
    overall_min -= 5.0;
    overall_max += 5.0;
    
    // Визуализируем разные траектории
    visualizeVector(clean_trajectory, "Чистая траектория (без шума)", overall_min, overall_max);
    visualizeVector(noisy_trajectory, "Зашумленная траектория", overall_min, overall_max);
    
    // Сглаживаем с оптимальным окном (например, k=2, окно 5)
    std::vector<double> smoothed_optimal = movingAverage(noisy_trajectory, 2, true);
    visualizeVector(smoothed_optimal, "Сглаженная траектория (окно 5)", overall_min, overall_max);
    
    // Тест 4: Сравнение производительности методов
    std::cout << "\n\n=== ТЕСТ 4: СРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ ===" << std::endl;
    
    // Создаем большой массив для тестирования производительности
    std::cout << "\nТестирование на большом массиве (10000 точек):" << std::endl;
    
    std::vector<double> large_data = generateCleanTrajectory(10000, 60.0, 9.8, 0.01);
    large_data = addNoise(large_data, 3.0, 54321);
    
    int test_k = 10;  // Окно размером 21
    
    // Измеряем время для простого метода
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<double> result_simple = movingAverage(large_data, test_k, false);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_simple = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    // Измеряем время для метода с префиксными суммами
    start = std::chrono::high_resolution_clock::now();
    std::vector<double> result_prefix = movingAverage(large_data, test_k, true);
    end = std::chrono::high_resolution_clock::now();
    auto duration_prefix = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << "Размер данных: " << large_data.size() << " точек" << std::endl;
    std::cout << "Размер окна: " << (2*test_k + 1) << " точек" << std::endl;
    std::cout << "\nРезультаты измерения времени:" << std::endl;
    std::cout << "Простой метод: " << duration_simple.count() << " мкс" << std::endl;
    std::cout << "Метод с префиксными суммами: " << duration_prefix.count() << " мкс" << std::endl;
    
    double speedup = static_cast<double>(duration_simple.count()) / duration_prefix.count();
    std::cout << "Ускорение: " << speedup << " раз" << std::endl;
    
    // Проверяем, что результаты идентичны
    bool identical = true;
    double max_diff = 0.0;
    for (size_t i = 0; i < result_simple.size(); ++i) {
        double diff = fabs(result_simple[i] - result_prefix[i]);
        if (diff > 1e-10) {
            identical = false;
            max_diff = std::max(max_diff, diff);
        }
    }
    
    if (identical) {
        std::cout << "✓ Результаты обоих методов идентичны" << std::endl;
    } else {
        std::cout << "⚠ Результаты немного отличаются (макс. разница: " << max_diff << ")" << std::endl;
    }
    
    // Тест 5: Практический пример - сглаживание реальных данных
    std::cout << "\n\n=== ТЕСТ 5: ПРАКТИЧЕСКИЙ ПРИМЕР ===" << std::endl;
    
    // Имитация реальных измерений высоты ракеты
    std::vector<double> real_measurements = {
        0.0, 8.2, 16.1, 23.5, 30.2, 36.1, 41.2, 45.5, 48.9, 51.4,
        53.0, 53.7, 53.5, 52.4, 50.4, 47.5, 43.8, 39.2, 33.8, 27.6,
        20.6, 12.8, 4.3, -4.9, -14.9, -25.5, -36.8, -48.7, -61.2, -74.3
    };
    
    std::cout << "\nРеальные измерения высоты ракеты (30 точек):" << std::endl;
    
    // Применяем разные методы сглаживания
    std::vector<double> smoothed_real_3 = movingAverage3(real_measurements, true);
    std::vector<double> smoothed_real_5 = movingAverage(real_measurements, 2, true);  // окно 5
    std::vector<double> smoothed_real_7 = movingAverage(real_measurements, 3, true);  // окно 7
    
    // Выводим результаты
    std::cout << "\nСравнение методов сглаживания для реальных данных:" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    std::cout << std::setw(10) << "Индекс" 
              << std::setw(15) << "Измерение" 
              << std::setw(15) << "Окно 3" 
              << std::setw(15) << "Окно 5" 
              << std::setw(15) << "Окно 7" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    for (size_t i = 0; i < std::min((size_t)15, real_measurements.size()); ++i) {
        std::cout << std::fixed << std::setprecision(2)
                  << std::setw(10) << i
                  << std::setw(15) << real_measurements[i]
                  << std::setw(15) << smoothed_real_3[i]
                  << std::setw(15) << smoothed_real_5[i]
                  << std::setw(15) << smoothed_real_7[i] << std::endl;
    }
    std::cout << std::string(100, '-') << std::endl;
    
    // Анализ результатов
    std::cout << "\nАнализ результатов:" << std::endl;
    
    // Вычисляем "шумность" каждого результата
    auto calculateNoisiness = [](const std::vector<double>& data) {
        double sum_abs_diff = 0.0;
        for (size_t i = 1; i < data.size(); ++i) {
            sum_abs_diff += fabs(data[i] - data[i-1]);
        }
        return sum_abs_diff / (data.size() - 1);
    };
    
    double noisiness_original = calculateNoisiness(real_measurements);
    double noisiness_window3 = calculateNoisiness(smoothed_real_3);
    double noisiness_window5 = calculateNoisiness(smoothed_real_5);
    double noisiness_window7 = calculateNoisiness(smoothed_real_7);
    
    std::cout << "Среднее абсолютное изменение между точками:" << std::endl;
    std::cout << "  Исходные данные: " << noisiness_original << std::endl;
    std::cout << "  Окно 3: " << noisiness_window3 
              << " (уменьшение на " << (noisiness_original - noisiness_window3)/noisiness_original*100 << "%)" << std::endl;
    std::cout << "  Окно 5: " << noisiness_window5 
              << " (уменьшение на " << (noisiness_original - noisiness_window5)/noisiness_original*100 << "%)" << std::endl;
    std::cout << "  Окно 7: " << noisiness_window7 
              << " (уменьшение на " << (noisiness_original - noisiness_window7)/noisiness_original*100 << "%)" << std::endl;
    
    // Выводы и рекомендации
    std::cout << "\n\n=== ВЫВОДЫ И РЕКОМЕНДАЦИИ ===" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    std::cout << "1. Методы сглаживания:" << std::endl;
    std::cout << "   - Скользящее среднее с окном 3: быстро, но менее эффективно" << std::endl;
    std::cout << "   - Скользящее среднее с произвольным окном: более гибко" << std::endl;
    std::cout << "   - Метод с префиксными суммами: эффективен для больших окон" << std::endl;
    std::cout << "\n2. Выбор размера окна:" << std::endl;
    std::cout << "   - Малое окно (3-5): сохраняет детали, но хуже убирает шум" << std::endl;
    std::cout << "   - Среднее окно (5-9): баланс между сглаживанием и сохранением формы" << std::endl;
    std::cout << "   - Большое окно (>9): сильно сглаживает, может искажать форму" << std::endl;
    std::cout << "\n3. Рекомендации:" << std::endl;
    std::cout << "   - Для быстрой обработки используйте метод с префиксными суммами" << std::endl;
    std::cout << "   - Экспериментируйте с размером окна для ваших данных" << std::endl;
    std::cout << "   - Используйте MSE для количественной оценки качества сглаживания" << std::endl;
    std::cout << std::string(60, '=') << std::endl;
    
    return 0;
}