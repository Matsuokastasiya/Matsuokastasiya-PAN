#include <iostream>
#include <iomanip>

class Engine {
private:
    double thrust;
    double fuelFlow;

public:
    // Конструктор
    Engine(double T, double F) : thrust(T), fuelFlow(F) {}

    // Возвращает удельный импульс
    double getSpecificImpulse() {
        if (fuelFlow == 0) {
            return 0; // чтобы избежать деления на ноль
        }
        return thrust / (fuelFlow * 9.81);
    }

    // Выводит информацию о двигателе
    void printlnfo() {
        double specificImpulse = getSpecificImpulse();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Тяга: " << thrust << " H | Расход: " << fuelFlow 
                  << " кг/с | Удельный импульс: " << std::setprecision(2) 
                  << specificImpulse << " с" << std::endl;
    }
};

int main() {
    Engine el(5000, 2.5);
    el.printlnfo();

    return 0;
}