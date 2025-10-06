#include <iostream> // input-output library
#include <cmath> // iclude math

int main(){
    double h;
    double a_y;
    double t;

    std::cout << "Введите высоту h (м): ";
    std::cin >> h;
    
    std::cout << "Введите вертикальное ускорение a_y (м/с²): ";
    std::cin >> a_y;

    if (h <= 0) {
        std::cout << "Ошибка: высота должна быть положительным числом!" <<  std::endl;
        return 1;
    }

    if (a_y <= 0) {
         std::cout << "Ошибка: вертикальное ускорение должно быть положительным числом!" <<  std::endl;
         std::cout << "Для набора высоты ускорение должно быть направлено вверх (a_y > 0)." <<  std::endl;
        return 1;
    }

    t = sqrt(2 * h / a_y);
    std::cout << "t = " << t << " с" << std::endl;
    return 0;
}