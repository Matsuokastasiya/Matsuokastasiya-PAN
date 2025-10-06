#include <iostream>
#include <cmath>
#include <iomanip>

const double g = 9.81;
const double RHO = 1.225;

int main() {
    // Параметры самолета
    double mass = 12000, wing_area = 28, velocity = 150, cl = 1.2, cd = 0.025;
    double target_height = 1000;
    
    // Диапазон тяги
    double min_thrust = 30000, max_thrust = 100000, step = 5000;
    
    // Расчет постоянных величин
    double lift = 0.5 * RHO * velocity * velocity * wing_area * cl;
    double drag = 0.5 * RHO * velocity * velocity * wing_area * cd;
    double weight = mass * g;
    
    // Поиск оптимальной тяги
    double best_thrust = min_thrust;
    double best_time = 1e9;
    
    std::cout << "Тяга (кН)\tВремя (с)\tСтатус\n";
    std::cout << "---------------------------------\n";
    
    for (double thrust = min_thrust; thrust <= max_thrust; thrust += step) {
        double acceleration = (lift - weight) / mass; // тяга не влияет на вертикальное ускорение!
        double time;
        
        if (acceleration > 0) {
            time = sqrt(2 * target_height / acceleration);
            std::cout << thrust/1000 << "\t\t" << time << "\t\tВОЗМОЖНО";
            
            if (time < best_time) {
                best_time = time;
                best_thrust = thrust;
                std::cout << " *ЛУЧШИЙ*";
            }
        } else {
            time = -1;
            std::cout << thrust/1000 << "\t\t---\t\tНЕВОЗМОЖНО";
        }
        std::cout << std::endl;
    }
    
    std::cout << "\nОПТИМАЛЬНЫЙ РЕЗУЛЬТАТ:\n";
    std::cout << "Тяга: " << best_thrust/1000 << " кН\n";
    std::cout << "Время: " << best_time << " с\n";
    
    return 0;
}