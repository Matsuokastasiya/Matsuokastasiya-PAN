#include <iostream> // input-output library
#include <cmath> // iclude math

int main(){
    double m;    
    double T;    
    double D;    
    double L;    
    double a;    
    double a_y; 
    const double g = 9.81;

    std::cout << "Друг мой, будь осторожен, масса не может быть отрицательной. Введите массу ЛА m (кг): ";
    std::cin >> m;
    
    std::cout << "Введите тягу двигателя T (Н): ";
    std::cin >> T;
    
    std::cout << "Введите сопротивление D (Н): ";
    std::cin >> D;
    
    std::cout << "Введите подъемную силу L (Н): ";
    std::cin >> L;

    a = (T - D) / m;           // Ускорение по направлению движения
    a_y = (L - m * g) / m;     // Вертикальное ускорение

    std::cout << "Ускорение по направлению движения" << a << "м/с²" << std::endl;
    std::cout << "Вертикальное ускорение" << a_y << "м/с²" << std::endl;

     if (a > 0) {
        std::cout << "Самолет ускоряется вперед" << std::endl;
    } else if (a < 0) {
       std::cout << "Самолет замедляется" << std::endl;
    } else {
        std::cout << "Скорость постоянна" << std::endl;
    }

    if (a_y > 0) {
        std::cout << "Самолет набирает высоту" << std::endl;
    } else if (a_y < 0) {
        std::cout << "Самолет теряет высоту" << std::endl;
    } else {
        std::cout << "Полет на постоянной высоте" << std::endl;
    }
    return 0;
}