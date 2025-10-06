#include <iostream> // input-output library
#include <cmath> // include math
#include <string> 
#include <iomanip>

const double g = 9.81;

// Функция для расчета ускорения
double calculateAcceleration(double thrust, double drag, double mass) {
    return (thrust - drag) / mass;
}

// Функция для определения режима полета
std::string determineFlightMode(double acceleration) {
    if (acceleration > 0.5) {
        return "набор высоты";
    } else if (acceleration >= 0 && acceleration <= 0.5) {
        return "горизонтальный полет";
    } else {
        return "снижение";
    }
}

int main() {
    // Параметры самолета
    double mass, thrust, lift, drag;
    
    std::cout << "=== АВТОМАТИЧЕСКИЙ ВЫБОР РЕЖИМА ПОЛЕТА ===" << std::endl;
    std::cout << std::endl;
    
    // Ввод данных от пользователя
    std::cout << "Введите параметры самолета:" << std::endl;
    std::cout << "Масса самолета (кг): ";
    std::cin >> mass;
    std::cout << "Тяга двигателя (Н): ";
    std::cin >> thrust;
    std::cout << "Подъемная сила (Н): ";
    std::cin >> lift;
    std::cout << "Сила сопротивления (Н): ";
    std::cin >> drag;
    
    // Проверка корректности введенных данных
    if (mass <= 0) {
        std::cout << "Ошибка: масса должна быть положительной!" << std::endl;
        return 1;
    }
    
    // Расчет ускорения
    double acceleration = calculateAcceleration(thrust, drag, mass);
    
    // Определение режима полета
    std::string flightMode = determineFlightMode(acceleration);
    
    // Вывод результатов
    std::cout << std::endl;
    std::cout << "=== РЕЗУЛЬТАТЫ РАСЧЕТА ===" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Ускорение самолета: " << acceleration << " м/с²" << std::endl;
    std::cout << "Режим полета: " << flightMode << std::endl;
    
    return 0;
}