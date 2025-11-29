#include <iostream>

class Sensor {
private:
    double signalStrength;
    bool isActive;

public:
    // Конструктор
    Sensor(double strength) {
        setSignalStrength(strength);
        activate();
    }

    // Включает датчик
    void activate() {
        isActive = true;
    }

    // Выключает датчик
    void deactivate() {
        isActive = false;
    }

    // Устанавливает новое значение силы сигнала
    void setSignalStrength(double s) {
        if (s < 0) {
            signalStrength = 0;
        } else {
            signalStrength = s;
        }
    }

    // Возвращает текущую силу сигнала
    double getSignalStrength() {
        return signalStrength;
    }

    // Выводит состояние датчика
    void printStatus() {
        if (isActive) {
            std::cout << "Датчик активен | Сила сигнала: " << signalStrength << " дБ" << std::endl;
        } else {
            std::cout << "Датчик выключен | Сила сигнала: " << signalStrength << " дБ" << std::endl;
        }
    }
};

int main() {
    Sensor s1(45.5);
    s1.printStatus();
    
    s1.setSignalStrength(-10);
    s1.printStatus();
    
    s1.deactivate();
    s1.printStatus();

    return 0;
}