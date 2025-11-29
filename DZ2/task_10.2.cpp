#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

class DroneFlight {
private:
    std::vector<double> x;
    std::vector<double> y;
    double totalDistance;

public:
    // Конструктор
    DroneFlight() : totalDistance(0) {
        // Начальная точка (0, 0)
        x.push_back(0);
        y.push_back(0);
    }

    // Добавляет координату и увеличивает totalDistance
    void addPoint(double newX, double newY) {
        // Вычисляем расстояние от последней точки до новой
        if (!x.empty()) {
            double lastX = x.back();
            double lastY = y.back();
            double distance = std::sqrt(std::pow(newX - lastX, 2) + std::pow(newY - lastY, 2));
            totalDistance += distance;
        }
        
        // Добавляем новую точку
        x.push_back(newX);
        y.push_back(newY);
    }

    // Возвращает суммарное расстояние
    double getTotalDistance() {
        return totalDistance;
    }

    // Выводит все точки маршрута
    void printPath() {
        std::cout << "Точки маршрута:" << std::endl;
        for (size_t i = 0; i < x.size(); ++i) {
            std::cout << "(" << x[i] << ", " << y[i] << ")" << std::endl;
        }
    }
};

int main() {
    DroneFlight d;
    d.addPoint(0, 0);
    d.addPoint(3, 4);
    d.addPoint(6, 8);
    d.printPath();
    std::cout << "Пройдено: " << d.getTotalDistance() << " м" << std::endl;

    return 0;
}