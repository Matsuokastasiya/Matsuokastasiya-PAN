#include <iostream>
#include <iomanip>

class NavigationSystem {
private:
    double x, y;
    double velocityX, velocityY;
    bool gpsAvailable;

public:
    // Конструктор с параметрами
    NavigationSystem(double startX, double startY, double velX, double velY, bool gps) 
        : x(startX), y(startY), velocityX(velX), velocityY(velY), gpsAvailable(gps) {}

    // Обновляет координаты на основе скорости
    void integratePosition(double dt) {
        x += velocityX * dt;
        y += velocityY * dt;
    }

    // Корректирует позицию по GPS данным
    void correctGPS(double realX, double realY) {
        if (gpsAvailable) {
            x = (x + realX) / 2.0;
            y = (y + realY) / 2.0;
        }
    }

    // Выводит текущие координаты
    void printPosition() {
        std::cout << std::fixed << std::setprecision(0);
        std::cout << "Скорректированные координаты: (" << x << ", " << y << ")" << std::endl;
    }
};

int main() {
    NavigationSystem nav(0, 0, 100, 50, true);
    
    nav.integratePosition(1);
    nav.correctGPS(110, 60);
    nav.printPosition();

    return 0;
}