#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

// 1. Класс Engine (двигательная установка)
class Engine {
private:
    double thrust;      // текущая тяга (H)
    double fuelFlow;    // расход топлива (кг/с)
    double fuel;        // оставшееся топливо (кг)

public:
    // Конструктор с параметрами
    Engine(double t, double ff, double f) : thrust(t), fuelFlow(ff), fuel(f) {}

    // Возвращает текущую тягу
    double getThrust() {
        return thrust;
    }

    // Проверяет наличие топлива
    bool hasFuel() {
        return fuel > 0;
    }

    // Уменьшает топливо на fuelFlow * dt
    void burn(double dt) {
        if (hasFuel()) {
            double fuelConsumed = fuelFlow * dt;
            if (fuelConsumed > fuel) {
                fuelConsumed = fuel; // не допускаем отрицательное топливо
            }
            fuel -= fuelConsumed;
        }
    }

    // Возвращает оставшееся топливо (для итогового отчета)
    double getFuel() {
        return fuel;
    }
};

// 2. Класс Navigation (навигация)
class Navigation {
private:
    double altitude;
    double velocity;
    double acceleration;
    double mass;

public:
    // Конструктор
    Navigation(double alt, double vel, double m) : altitude(alt), velocity(vel), mass(m), acceleration(0) {}

    // Обновляет параметры полета
    void update(double thrust, double dt) {
        // a = thrust/mass - 9.81
        acceleration = thrust / mass - 9.81;
        
        // v += a * dt
        velocity += acceleration * dt;
        
        // h += v * dt
        altitude += velocity * dt;
        
        // Не допускаем отрицательную высоту
        if (altitude < 0) {
            altitude = 0;
            velocity = 0;
            acceleration = 0;
        }
    }

    // Выводит текущие параметры
    void printStatus(double time) {
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "t=" << time << "c | h=" << altitude << "м | v=" 
                  << velocity << "м/c | a=" << acceleration << "м/c²" << std::endl;
    }

    // Возвращает текущую высоту (для итогового отчета)
    double getAltitude() {
        return altitude;
    }
};

// 3. Класс AutonomousFlightSystem (автономная система управления)
class AutonomousFlightSystem {
private:
    Engine engine;
    Navigation nav;
    double time;

public:
    // Конструктор
    AutonomousFlightSystem(const Engine& e, const Navigation& n) : engine(e), nav(n), time(0) {}

    // Выполняет пошаговую симуляцию
    void simulate(double dt, double totalTime) {
        while (time < totalTime && engine.hasFuel()) {
            // Обновляем двигатель
            engine.burn(dt);
            
            // Обновляем навигацию
            nav.update(engine.getThrust(), dt);
            
            // Выводим статус
            nav.printStatus(time);
            
            // Увеличиваем время
            time += dt;
        }
    }

    // Итоговое сообщение о полете
    void printSummary() {
        std::cout << "--- Полет завершен ---" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Максимальная высота: " << nav.getAltitude() << " м" << std::endl;
        std::cout << "Оставшееся топливо: " << engine.getFuel() << " кг" << std::endl;
    }
};

int main() {
    // Создаем двигатель: тяга 50000 Н, расход 20 кг/с, топливо 100 кг
    Engine engine(50000, 20, 100);
    
    // Создаем навигацию: начальная высота 0 м, скорость 0 м/с, масса 1000 кг
    Navigation navigation(0, 0, 1000);
    
    // Создаем автономную систему управления
    AutonomousFlightSystem system(engine, navigation);
    
    // Запускаем симуляцию: шаг 1.0 с, общее время 10 с
    system.simulate(1.0, 10.0);
    
    // Выводим итоги
    system.printSummary();

    return 0;
}