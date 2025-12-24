#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <random>
#include <ctime>

// Функция для генерации случайных высот
std::vector<double> generateHeights(int count, double min_val, double max_val) {
    std::vector<double> heights;
    heights.reserve(count);
    
    std::mt19937 gen(static_cast<unsigned int>(time(nullptr)));
    std::uniform_real_distribution<double> dist(min_val, max_val);
    
    for (int i = 0; i < count; ++i) {
        heights.push_back(dist(gen));
    }
    
    return heights;
}

// Функция для вывода вектора
void printVector(const std::vector<double>& vec, const std::string& title) {
    std::cout << title << " (" << vec.size() << " элементов):" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    if (vec.empty()) {
        std::cout << "Вектор пуст" << std::endl;
    } else {
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << std::fixed << std::setprecision(2)
                      << "[" << i << "] = " << std::setw(8) << vec[i];
            
            // Выделяем значения ниже 10
            if (vec[i] < 10.0) {
                std::cout << "  <-- будет удалено";
            }
            std::cout << std::endl;
        }
    }
    std::cout << std::string(50, '-') << std::endl;
}

// Функция для удаления значений ниже порога
std::vector<double> removeValuesBelowThreshold(std::vector<double> heights, double threshold) {
    std::cout << "\nУдаление значений ниже " << threshold << " м:" << std::endl;
    
    // Сохраняем исходный размер
    size_t original_size = heights.size();
    
    // Используем std::remove_if для перемещения удаляемых элементов в конец
    auto new_end = std::remove_if(heights.begin(), heights.end(),
                                  [threshold](double h) { return h < threshold; });
    
    // Удаляем элементы от new_end до конца вектора
    heights.erase(new_end, heights.end());
    
    // Выводим статистику
    std::cout << "Удалено элементов: " << (original_size - heights.size()) << std::endl;
    std::cout << "Осталось элементов: " << heights.size() << std::endl;
    
    return heights;
}

int main() {
    std::cout << "=== ЗАДАЧА 6: УДАЛЕНИЕ ЗНАЧЕНИЙ ===" << std::endl;
    
    // Тест 1: Нормальный случай - смешанные значения
    std::cout << "\n\n=== ТЕСТ 1: Смешанные значения (0-20 м) ===" << std::endl;
    {
        std::vector<double> heights = generateHeights(15, 0.0, 20.0);
        printVector(heights, "Исходный вектор высот");
        
        std::vector<double> filtered = removeValuesBelowThreshold(heights, 10.0);
        printVector(filtered, "Вектор после удаления (h < 10 м)");
    }
    
    // Тест 2: Все значения ниже порога
    std::cout << "\n\n=== ТЕСТ 2: Все значения ниже 10 м ===" << std::endl;
    {
        std::vector<double> heights = generateHeights(10, 0.0, 9.9);
        printVector(heights, "Исходный вектор высот (все < 10 м)");
        
        std::vector<double> filtered = removeValuesBelowThreshold(heights, 10.0);
        printVector(filtered, "Вектор после удаления");
        
        if (filtered.empty()) {
            std::cout << "✓ Вектор стал пустым (как и ожидалось)" << std::endl;
        }
    }
    
    // Тест 3: Все значения выше порога
    std::cout << "\n\n=== ТЕСТ 3: Все значения выше 10 м ===" << std::endl;
    {
        std::vector<double> heights = generateHeights(10, 10.1, 30.0);
        printVector(heights, "Исходный вектор высот (все > 10 м)");
        
        // Сохраняем копию для сравнения
        std::vector<double> original_copy = heights;
        
        std::vector<double> filtered = removeValuesBelowThreshold(heights, 10.0);
        printVector(filtered, "Вектор после удаления");
        
        if (filtered.size() == original_copy.size()) {
            std::cout << "✓ Ничего не удалено (как и ожидалось)" << std::endl;
            
            // Проверяем, что элементы не изменились
            bool identical = true;
            for (size_t i = 0; i < filtered.size(); ++i) {
                if (filtered[i] != original_copy[i]) {
                    identical = false;
                    break;
                }
            }
            if (identical) {
                std::cout << "✓ Порядок элементов сохранен" << std::endl;
            }
        }
    }
    
    // Тест 4: Специальные случаи
    std::cout << "\n\n=== ТЕСТ 4: Специальные случаи ===" << std::endl;
    
    // 4.1: Значения равные порогу
    std::cout << "\n--- Значения равные порогу 10.0 ---" << std::endl;
    {
        std::vector<double> heights = {5.0, 10.0, 15.0, 10.0, 3.0, 10.0};
        printVector(heights, "Исходный вектор (с значениями = 10)");
        
        std::vector<double> filtered = removeValuesBelowThreshold(heights, 10.0);
        printVector(filtered, "После удаления (h < 10)");
        
        // Подсчет значений = 10
        int count_equal = 0;
        for (double h : filtered) {
            if (h == 10.0) count_equal++;
        }
        std::cout << "Значений равных 10.0: " << count_equal << " (должно остаться)" << std::endl;
    }
    
    // 4.2: Пустой вектор
    std::cout << "\n--- Пустой вектор ---" << std::endl;
    {
        std::vector<double> heights;
        printVector(heights, "Исходный пустой вектор");
        
        std::vector<double> filtered = removeValuesBelowThreshold(heights, 10.0);
        printVector(filtered, "После удаления");
        
        if (filtered.empty()) {
            std::cout << "✓ Пустой вектор остался пустым" << std::endl;
        }
    }
    
    // 4.3: Отрицательные значения
    std::cout << "\n--- Отрицательные значения ---" << std::endl;
    {
        std::vector<double> heights = {-5.0, -1.0, 0.0, 5.0, 10.0, 15.0, 20.0};
        printVector(heights, "Исходный вектор (с отрицательными)");
        
        std::vector<double> filtered = removeValuesBelowThreshold(heights, 10.0);
        printVector(filtered, "После удаления (h < 10)");
        
        // Проверяем, что остались только >= 10
        bool all_above_threshold = true;
        for (double h : filtered) {
            if (h < 10.0) {
                all_above_threshold = false;
                break;
            }
        }
        if (all_above_threshold) {
            std::cout << "✓ Все оставшиеся значения >= 10.0" << std::endl;
        }
    }
    
    // Демонстрация работы remove_if + erase
    std::cout << "\n\n=== КАК РАБОТАЕТ remove_if + erase ===" << std::endl;
    {
        std::vector<double> heights = {3.0, 12.0, 7.0, 15.0, 2.0, 20.0, 9.0};
        std::cout << "Исходный вектор: ";
        for (double h : heights) std::cout << h << " ";
        std::cout << std::endl;
        
        // Шаг 1: remove_if перемещает удаляемые элементы в конец
        auto new_end = std::remove_if(heights.begin(), heights.end(),
                                      [](double h) { return h < 10.0; });
        
        std::cout << "После remove_if (но ДО erase):" << std::endl;
        std::cout << "Весь вектор: ";
        for (double h : heights) std::cout << h << " ";
        std::cout << std::endl;
        
        std::cout << "Диапазон [begin, new_end): ";
        for (auto it = heights.begin(); it != new_end; ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Диапазон [new_end, end): ";
        for (auto it = new_end; it != heights.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Размер ДО erase: " << heights.size() << std::endl;
        std::cout << "Емкость ДО erase: " << heights.capacity() << std::endl;
        
        // Шаг 2: erase удаляет "хвост"
        heights.erase(new_end, heights.end());
        
        std::cout << "\nПосле erase:" << std::endl;
        std::cout << "Весь вектор: ";
        for (double h : heights) std::cout << h << " ";
        std::cout << std::endl;
        std::cout << "Размер ПОСЛЕ erase: " << heights.size() << std::endl;
        std::cout << "Емкость ПОСЛЕ erase: " << heights.capacity() << std::endl;
        
        std::cout << "\nВажно: capacity обычно не уменьшается после erase!" << std::endl;
    }
    
    // Альтернативные способы удаления
    std::cout << "\n\n=== АЛЬТЕРНАТИВНЫЕ СПОСОБЫ УДАЛЕНИЯ ===" << std::endl;
    
    std::vector<double> heights = {3.0, 12.0, 7.0, 15.0, 2.0, 20.0, 9.0, 25.0, 1.0};
    std::cout << "Исходный вектор: ";
    for (double h : heights) std::cout << h << " ";
    std::cout << std::endl;
    
    // Способ 1: Классический цикл (менее эффективный)
    std::cout << "\n--- Способ 1: Классический цикл с erase ---" << std::endl;
    {
        std::vector<double> h1 = heights;
        
        for (auto it = h1.begin(); it != h1.end(); ) {
            if (*it < 10.0) {
                it = h1.erase(it);  // erase возвращает итератор на следующий элемент
            } else {
                ++it;
            }
        }
        
        std::cout << "Результат: ";
        for (double h : h1) std::cout << h << " ";
        std::cout << std::endl;
        std::cout << "Недостаток: O(n²) в худшем случае из-за сдвига элементов" << std::endl;
    }
    
    // Способ 2: remove_if с лямбдой (рекомендуемый)
    std::cout << "\n--- Способ 2: remove_if + erase (рекомендуемый) ---" << std::endl;
    {
        std::vector<double> h2 = heights;
        
        h2.erase(std::remove_if(h2.begin(), h2.end(),
                                [](double h) { return h < 10.0; }),
                 h2.end());
        
        std::cout << "Результат: ";
        for (double h : h2) std::cout << h << " ";
        std::cout << std::endl;
        std::cout << "Преимущество: O(n), оптимально по производительности" << std::endl;
    }
    
    // Способ 3: Копирование в новый вектор
    std::cout << "\n--- Способ 3: Копирование в новый вектор ---" << std::endl;
    {
        std::vector<double> h3;
        h3.reserve(heights.size());  // резервируем память для эффективности
        
        std::copy_if(heights.begin(), heights.end(), std::back_inserter(h3),
                    [](double h) { return h >= 10.0; });
        
        std::cout << "Результат: ";
        for (double h : h3) std::cout << h << " ";
        std::cout << std::endl;
        std::cout << "Преимущество: сохраняет исходный вектор неизменным" << std::endl;
    }
    
    // Практический пример: фильтрация данных траектории
    std::cout << "\n\n=== ПРАКТИЧЕСКИЙ ПРИМЕР: Фильтрация траектории ===" << std::endl;
    
    // Генерируем данные траектории ракеты
    std::vector<double> trajectory_heights;
    const int num_points = 20;
    
    for (int i = 0; i < num_points; ++i) {
        double t = i * 0.5;  // время в секундах
        double h = 50.0 * t - 0.5 * 9.8 * t * t;  // формула высоты
        
        // Добавляем немного шума
        static std::mt19937 gen(42);
        std::normal_distribution<double> noise(0.0, 2.0);
        h += noise(gen);
        
        trajectory_heights.push_back(h);
    }
    
    std::cout << "Высоты ракеты по времени:" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    for (size_t i = 0; i < trajectory_heights.size(); ++i) {
        std::cout << "t=" << std::setw(4) << (i * 0.5) << " с: "
                  << std::fixed << std::setprecision(2) 
                  << std::setw(8) << trajectory_heights[i] << " м";
        if (trajectory_heights[i] < 10.0) {
            std::cout << "  (низкая высота)";
        }
        std::cout << std::endl;
    }
    std::cout << std::string(40, '-') << std::endl;
    
    // Фильтруем: удаляем значения ниже 10 м
    auto filtered_trajectory = removeValuesBelowThreshold(trajectory_heights, 10.0);
    
    std::cout << "\nТраектория после удаления низких высот (< 10 м):" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    if (filtered_trajectory.empty()) {
        std::cout << "Все высоты были ниже 10 м!" << std::endl;
    } else {
        for (size_t i = 0; i < filtered_trajectory.size(); ++i) {
            std::cout << "[" << i << "]: " 
                      << std::fixed << std::setprecision(2)
                      << std::setw(8) << filtered_trajectory[i] << " м" << std::endl;
        }
    }
    std::cout << std::string(40, '-') << std::endl;
    
    return 0;
}