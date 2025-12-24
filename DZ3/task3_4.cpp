#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <numeric>  // для std::accumulate

int main() {
    // Константы (как в задаче 3)
    const double v0 = 50.0;      // начальная скорость (м/с)
    const double g = 9.8;        // ускорение свободного падения (м/с²)
    const double dt = 0.5;       // шаг времени (с)
    const double t_max = 15.0;   // максимальное время моделирования (с)
    
    // Создание векторов для хранения времени и скорости (как в задаче 3)
    std::vector<double> times;
    std::vector<double> velocities;
    
    // Заполнение векторов данными (как в задаче 3)
    for (double t = 0.0; t <= t_max; t += dt) {
        double v = v0 - g * t;
        times.push_back(t);
        velocities.push_back(v);
    }
    
    // Проверка наличия данных
    if (velocities.empty()) {
        std::cout << "Вектор скоростей пуст. Нет данных для расчета." << std::endl;
        return 0;
    }
    
    std::cout << std::fixed << std::setprecision(2);
    
    // --- ВАРИАНТ 1: Простое среднее арифметическое ---
    std::cout << "=== ВАРИАНТ 1: Среднее арифметическое значение скоростей ===" << std::endl;
    std::cout << "Используется формула: avg = (1/N) * Σ v_i" << std::endl;
    std::cout << "где N = " << velocities.size() << " - количество измерений" << std::endl;
    
    // Использование std::accumulate для суммирования значений
    double sum = std::accumulate(velocities.begin(), velocities.end(), 0.0);
    double avg_arithmetic = sum / velocities.size();
    
    std::cout << "Сумма всех скоростей: " << sum << " м/с" << std::endl;
    std::cout << "Количество измерений: " << velocities.size() << std::endl;
    std::cout << "Средняя скорость (арифметическая): " << avg_arithmetic << " м/с" << std::endl;
    std::cout << std::endl;
    
    // --- ВАРИАНТ 2: Интегральное среднее (более физическое) ---
    std::cout << "=== ВАРИАНТ 2: Интегральное среднее значение ===" << std::endl;
    std::cout << "Используется формула: avg = (1/T) * ∫ v(t) dt" << std::endl;
    std::cout << "где T = " << times.back() << " с - полное время полета" << std::endl;
    
    // Вычисление интеграла методом трапеций
    double integral = 0.0;
    for (size_t i = 1; i < velocities.size(); ++i) {
        double trapezoid_area = (velocities[i-1] + velocities[i]) * (times[i] - times[i-1]) / 2.0;
        integral += trapezoid_area;
    }
    
    double total_time = times.back() - times.front();
    double avg_integral = integral / total_time;
    
    std::cout << "Интеграл скорости по времени: " << integral << " м" << std::endl;
    std::cout << "Полное время полета: " << total_time << " с" << std::endl;
    std::cout << "Средняя скорость (интегральная): " << avg_integral << " м/с" << std::endl;
    std::cout << std::endl;
    
    // --- ВАРИАНТ 3: Среднее с использованием теоретической формулы ---
    std::cout << "=== ВАРИАНТ 3: Теоретическое среднее значение ===" << std::endl;
    std::cout << "Для равномерно замедленного движения с v(t) = v0 - g*t:" << std::endl;
    std::cout << "Теоретическое среднее: avg_theoretical = (v_начальная + v_конечная) / 2" << std::endl;
    
    double v_initial = velocities.front();
    double v_final = velocities.back();
    double avg_theoretical = (v_initial + v_final) / 2.0;
    
    std::cout << "Начальная скорость: " << v_initial << " м/с" << std::endl;
    std::cout << "Конечная скорость: " << v_final << " м/с" << std::endl;
    std::cout << "Теоретическое среднее: (" << v_initial << " + " << v_final 
              << ") / 2 = " << avg_theoretical << " м/с" << std::endl;
    std::cout << std::endl;
    
    // --- Сравнение результатов ---
    std::cout << "=== СРАВНЕНИЕ РЕЗУЛЬТАТОВ ===" << std::endl;
    std::cout << std::setw(25) << "Метод расчета" << std::setw(20) << "Средняя скорость" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    std::cout << std::setw(25) << "Арифметическое среднее" 
              << std::setw(20) << avg_arithmetic << " м/с" << std::endl;
    std::cout << std::setw(25) << "Интегральное среднее" 
              << std::setw(20) << avg_integral << " м/с" << std::endl;
    std::cout << std::setw(25) << "Теоретическое среднее" 
              << std::setw(20) << avg_theoretical << " м/с" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    
    // --- Анализ точности ---
    std::cout << "\n=== АНАЛИЗ ТОЧНОСТИ ===" << std::endl;
    std::cout << "Разница между методами:" << std::endl;
    std::cout << "|Арифм. - Интегр.| = " << fabs(avg_arithmetic - avg_integral) << " м/с" << std::endl;
    std::cout << "|Арифм. - Теор.| = " << fabs(avg_arithmetic - avg_theoretical) << " м/с" << std::endl;
    std::cout << "|Интегр. - Теор.| = " << fabs(avg_integral - avg_theoretical) << " м/с" << std::endl;
    
    // Определение наиболее точного метода (ближайшего к теоретическому)
    double diff_arith = fabs(avg_arithmetic - avg_theoretical);
    double diff_integ = fabs(avg_integral - avg_theoretical);
    
    if (diff_arith < diff_integ) {
        std::cout << "Арифметическое среднее ближе к теоретическому значению." << std::endl;
    } else if (diff_integ < diff_arith) {
        std::cout << "Интегральное среднее ближе к теоретическому значению." << std::endl;
    } else {
        std::cout << "Оба метода дают одинаковую точность." << std::endl;
    }
    
    // --- Дополнительные расчеты с использованием std::accumulate ---
    std::cout << "\n=== ДОПОЛНИТЕЛЬНЫЕ РАСЧЕТЫ С std::accumulate ===" << std::endl;
    
    // 1. Сумма абсолютных значений скоростей
    double sum_abs = std::accumulate(velocities.begin(), velocities.end(), 0.0,
                                     [](double acc, double v) { return acc + fabs(v); });
    std::cout << "Сумма абсолютных значений скоростей: " << sum_abs << " м/с" << std::endl;
    
    // 2. Среднее абсолютное значение скорости
    double avg_abs = sum_abs / velocities.size();
    std::cout << "Среднее абсолютное значение скорости: " << avg_abs << " м/с" << std::endl;
    
    // 3. Сумма квадратов скоростей
    double sum_squares = std::accumulate(velocities.begin(), velocities.end(), 0.0,
                                        [](double acc, double v) { return acc + v * v; });
    std::cout << "Сумма квадратов скоростей: " << sum_squares << " м²/с²" << std::endl;
    
    // 4. Среднеквадратичная скорость
    double rms = sqrt(sum_squares / velocities.size());
    std::cout << "Среднеквадратичная скорость (RMS): " << rms << " м/с" << std::endl;
    
    // --- Вывод первых 5 значений для проверки ---
    std::cout << "\n=== ПЕРВЫЕ 5 ЗНАЧЕНИЙ СКОРОСТИ ===" << std::endl;
    std::cout << std::setw(10) << "Индекс" << std::setw(15) << "Время (с)" 
              << std::setw(20) << "Скорость (м/с)" << std::endl;
    std::cout << std::string(45, '-') << std::endl;
    
    size_t points_to_show = std::min((size_t)5, velocities.size());
    for (size_t i = 0; i < points_to_show; ++i) {
        std::cout << std::setw(10) << i 
                  << std::setw(15) << times[i] 
                  << std::setw(20) << velocities[i] << std::endl;
    }
    
    if (velocities.size() > 5) {
        std::cout << "..." << std::endl;
        std::cout << std::setw(10) << velocities.size()-1 
                  << std::setw(15) << times.back() 
                  << std::setw(20) << velocities.back() << std::endl;
    }
    
    // --- Физическая интерпретация ---
    std::cout << "\n=== ФИЗИЧЕСКАЯ ИНТЕРПРЕТАЦИЯ ===" << std::endl;
    std::cout << "Для ракеты, запущенной вертикально вверх:" << std::endl;
    std::cout << "- Положительная скорость: движение вверх" << std::endl;
    std::cout << "- Отрицательная скорость: движение вниз" << std::endl;
    std::cout << "- Средняя скорость " << avg_integral << " м/с означает:" << std::endl;
    
    if (avg_integral > 0) {
        std::cout << "  В среднем ракета двигалась ВВЕРХ" << std::endl;
    } else if (avg_integral < 0) {
        std::cout << "  В среднем ракета двигалась ВНИЗ" << std::endl;
    } else {
        std::cout << "  Среднее перемещение равно нулю" << std::endl;
    }
    
    // Расчет общего перемещения
    double displacement = integral;  // интеграл скорости = перемещение
    std::cout << "Общее перемещение (интеграл скорости): " << displacement << " м" << std::endl;
    
    if (displacement > 0) {
        std::cout << "Итоговое положение: ВЫШЕ точки старта" << std::endl;
    } else if (displacement < 0) {
        std::cout << "Итоговое положение: НИЖЕ точки старта" << std::endl;
    } else {
        std::cout << "Итоговое положение: В точке старта" << std::endl;
    }
    
    return 0;
}