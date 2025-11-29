#include <iostream>
#include <iomanip>

class FlightComputer {
private:
    double altitude;
    double velocity;
    double fuel;
    double thrust;

public:
    // Конструктор с параметрами
    FlightComputer(double alt, double vel, double f, double t) 
        : altitude(alt), velocity(vel), fuel(f), thrust(t) {}

    // Симуляция одного шага времени
    void simulateStep(double dt) {
        // Обновляем скорость (учитываем тягу и гравитацию)
        velocity += (thrust / 1000 - 9.8) * dt;
        
        // Обновляем высоту
        altitude += velocity * dt;
        
        // Уменьшаем топливо
        fuel -= dt * 2;
        
        // Не допускаем отрицательное топливо
        if (fuel < 0) {
            fuel = 0;
        }
    }

    // Вывод состояния полета
    void printStatus() {
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "высота=" << altitude << "м, скорость=" << velocity 
                  << "м/с, топливо=" << fuel << std::endl;
    }
};

int main() {
    FlightComputer fc(0, 0, 100, 12000);
    
    for (int i = 0; i < 5; i++) {
        double time = (i + 1) * 0.5;
        std::cout << "t=" << time << "c: ";
        fc.simulateStep(0.5);
        fc.printStatus();
    }

    return 0;
}