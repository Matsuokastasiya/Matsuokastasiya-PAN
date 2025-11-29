#include <iostream>
#include <iomanip>

class AutonomousControl {
private:
    double altitude;
    double thrust;
    double targetAltitude;

public:
    // Конструктор с параметрами
    AutonomousControl(double alt, double th, double targetAlt) 
        : altitude(alt), thrust(th), targetAltitude(targetAlt) {}

    // Обновление управления тягой
    void updateControl() {
        if (altitude < targetAltitude) {
            thrust += 100; // увеличить тягу
        } else {
            thrust -= 100; // уменьшить тягу
        }
        
        // Ограничиваем тягу минимальным значением (не может быть отрицательной)
        if (thrust < 0) {
            thrust = 0;
        }
    }

    // Симуляция одного шага времени
    void simulateStep(double dt) {
        altitude += (thrust / 500) * dt;
    }

    // Вывод текущего статуса
    void printStatus() {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "Высота: " << altitude << "м, тяга: " << thrust << std::endl;
    }
};

int main() {
    AutonomousControl ac(0, 2000, 5000);
    
    for (int i = 0; i < 5; ++i) {
        ac.updateControl();
        ac.simulateStep(1.0);
        ac.printStatus();
    }

    return 0;
}