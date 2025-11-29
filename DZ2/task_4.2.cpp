#include <iostream>

class Gyroscope {
private:
    double angularVelocity;
    bool calibrationRequired;

public:
    // Конструктор
    Gyroscope(double velocity, bool calibration) 
        : angularVelocity(velocity), calibrationRequired(calibration) {}

    // Калибровка гироскопа
    void calibrate() {
        calibrationRequired = false;
    }

    // Вывод текущего состояния
    void printStatus() {
        std::cout << "Скорость: " << angularVelocity << " °/с | Калибровка ";
        if (calibrationRequired) {
            std::cout << "требуется";
        } else {
            std::cout << "не требуется";
        }
        std::cout << std::endl;
    }
};

int main() {
    Gyroscope g1(120.5, true);
    g1.printStatus();
    g1.calibrate();
    g1.printStatus();

    return 0;
}