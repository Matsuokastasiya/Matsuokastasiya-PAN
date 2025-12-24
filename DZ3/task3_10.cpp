#include <iostream>
#include <vector>
#include <cmath>

// Структура для хранения точки в пространстве
struct Point {
    double x, y, z;
    Point(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    // Для удобного вывода
    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "(" << p.x << ", " << p.y << ", " << p.z << ")";
        return os;
    }
};

// Класс ракеты
class Rocket {
private:
    Point position;     // текущее положение
    Point velocity;     // текущая скорость
    double acceleration; // ускорение двигателя (м/с²)
    double mass;        // масса ракеты (кг)
    double fuel;        // текущее количество топлива

public:
    // Конструктор
    Rocket(double startX, double startY, double startZ, 
           double startVx, double startVy, double startVz, 
           double accel = 20.0, double m = 1000.0, double f = 500.0)
        : position(startX, startY, startZ), 
          velocity(startVx, startVy, startVz), 
          acceleration(accel), mass(m), fuel(f) {}
    
    // Конструктор для упрощенного создания (как в задании)
    Rocket(double startX, double startVy, double startVz, double startVx)
        : position(startX, 0, 0), 
          velocity(startVx, startVy, startVz), 
          acceleration(20.0), mass(1000.0), fuel(500.0) {}

    // Обновление позиции за время dt
    void updatePosition(double dt) {
        // Обновляем позицию на основе текущей скорости
        position.x += velocity.x * dt;
        position.y += velocity.y * dt;
        position.z += velocity.z * dt;
        
        // Применяем ускорение двигателя (если есть топливо)
        if (fuel > 0) {
            double burnRate = 1.0; // кг/с
            double actualBurn = std::min(burnRate * dt, fuel);
            
            // Увеличиваем скорость в направлении полета
            double dv = (acceleration * actualBurn / mass) * dt;
            
            // Нормализуем вектор скорости для направления ускорения
            double speed = std::sqrt(velocity.x*velocity.x + 
                                     velocity.y*velocity.y + 
                                     velocity.z*velocity.z);
            if (speed > 0) {
                velocity.x += dv * velocity.x / speed;
                velocity.y += dv * velocity.y / speed;
                velocity.z += dv * velocity.z / speed;
            }
            
            fuel -= actualBurn;
        }
        
        // Простая гравитация (если ракета летит не вертикально)
        velocity.y -= 9.8 * dt * 0.1; // небольшая гравитация
    }

    // Изменение скорости (может использоваться для маневров)
    void changeVelocity(double dvx, double dvy, double dvz) {
        velocity.x += dvx;
        velocity.y += dvy;
        velocity.z += dvz;
    }

    // Геттеры для состояния
    Point getPosition() const { return position; }
    Point getVelocity() const { return velocity; }
    double getFuel() const { return fuel; }
    
    // Получение текущей скорости (скаляр)
    double getSpeed() const {
        return std::sqrt(velocity.x*velocity.x + 
                        velocity.y*velocity.y + 
                        velocity.z*velocity.z);
    }
};

int main() {
    // Создаем отряд из 5 ракет с разными начальными скоростями
    std::vector<Rocket> squad;
    squad.reserve(5);
    
    // Инициализируем 5 ракет (как в примере и еще 3)
    squad.emplace_back(1000, 10, 0, 50);   // Ракета 1
    squad.emplace_back(900, 15, -5, 40);   // Ракета 2
    squad.emplace_back(1100, 5, 10, 60);   // Ракета 3
    squad.emplace_back(950, -10, 5, 45);   // Ракета 4
    squad.emplace_back(1050, 0, -10, 55);  // Ракета 5
    
    // Вектор для хранения траекторий каждой ракеты
    std::vector<std::vector<Point>> trajectories(5);
    
    // Параметры симуляции
    const double T = 10.0;     // Общее время симуляции (10 секунд)
    const double dt = 0.1;     // Шаг по времени (0.1 секунды)
    
    std::cout << "=== Начало симуляции полета отряда ракет ===\n";
    std::cout << "Время симуляции: " << T << " секунд\n";
    std::cout << "Шаг симуляции: " << dt << " секунд\n\n";
    
    // Симуляция полета
    for (double t = 0; t <= T; t += dt) {
        for (size_t i = 0; i < squad.size(); ++i) {
            // Обновляем позицию текущей ракеты
            squad[i].updatePosition(dt);
            
            // Сохраняем текущую позицию в траекторию
            trajectories[i].push_back(squad[i].getPosition());
        }
    }
    
    // Выводим конечные положения всех ракет
    std::cout << "=== Конечные положения ракет после " << T << " секунд ===\n";
    for (size_t i = 0; i < squad.size(); ++i) {
        Point finalPos = squad[i].getPosition();
        double speed = squad[i].getSpeed();
        double fuel = squad[i].getFuel();
        
        std::cout << "Ракета " << (i + 1) << ":\n";
        std::cout << "  Положение: " << finalPos << "\n";
        std::cout << "  Скорость: " << speed << " м/с\n";
        std::cout << "  Остаток топлива: " << fuel << " кг\n";
        
        // Выводим несколько точек траектории для наглядности
        std::cout << "  Траектория (первые 3 точки): ";
        for (size_t j = 0; j < std::min(trajectories[i].size(), size_t(3)); ++j) {
            std::cout << trajectories[i][j];
            if (j < 2 && j < trajectories[i].size() - 1) std::cout << " -> ";
        }
        std::cout << "\n" << std::endl;
    }
    
    // Дополнительная статистика
    std::cout << "=== Статистика полета ===\n";
    double maxDistance = 0;
    int farthestRocket = 0;
    
    for (size_t i = 0; i < squad.size(); ++i) {
        Point pos = squad[i].getPosition();
        // Простая метрика расстояния от старта
        double distance = std::sqrt(pos.x*pos.x + pos.y*pos.y + pos.z*pos.z);
        if (distance > maxDistance) {
            maxDistance = distance;
            farthestRocket = i;
        }
    }
    
    std::cout << "Самую дальнюю дистанцию пролетела ракета " 
              << (farthestRocket + 1) 
              << " (" << maxDistance << " м от старта)\n";
    
    return 0;
}