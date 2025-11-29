#include <iostream>
#include <iomanip>

class RocketStage {
private:
    double thrust;
    double burnTime;
    double mass;

public:
    // Конструктор с параметрами
    RocketStage(double t, double bt, double m) : thrust(t), burnTime(bt), mass(m) {}

    // Вычисляет приращение скорости по формуле Циолковского
    double getDeltaV() {
        return (thrust * burnTime) / mass;
    }

    // Выводит данные ступени
    void printlnfo() {
        std::cout << "Тяга: " << thrust << " H, Время работы: " << burnTime 
                  << " с, Масса: " << mass << " кг, ΔV: " << std::fixed 
                  << std::setprecision(2) << getDeltaV() << " м/с" << std::endl;
    }
};

int main() {
    RocketStage s1(5000, 10, 1000);
    RocketStage s2(7000, 8, 900);
    RocketStage s3(9000, 6, 800);

    // Вывод информации о каждой ступени
    s1.printlnfo();
    s2.printlnfo();
    s3.printlnfo();

    // Расчет общей скорости
    double totalV = s1.getDeltaV() + s2.getDeltaV() + s3.getDeltaV();
    
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Итоговая скорость ракеты: " << totalV << " м/с\n";

    return 0;
}