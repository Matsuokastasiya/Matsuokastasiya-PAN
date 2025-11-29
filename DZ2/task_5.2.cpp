#include <iostream>

class Autopilot {
private:
    double courseAngle;
    double altitude;

public:
    // Конструктор с параметрами
    Autopilot(double course, double alt) : courseAngle(course), altitude(alt) {}

    // Изменение курса на заданную величину
    void changeCourse(double delta) {
        courseAngle += delta;
    }

    // Изменение высоты на заданную величину
    void changeAltitude(double delta) {
        altitude += delta;
    }

    // Вывод текущего статуса
    void printStatus() {
        std::cout << "Курс: " << courseAngle << "°, высота: " << altitude << " м" << std::endl;
    }
};

int main() {
    Autopilot a1(90, 1000);
    a1.changeCourse(10);
    a1.changeAltitude(500);
    a1.printStatus();

    return 0;
}