#include <iostream> // input-output library
#include <cmath> // iclude math

int main(){
     float rho;
    float v;
    float s;
    float cd;
    float L;
    std::cout << "Вводимое значение может быть только положительным числом.. Введите плотность воздуха r на высоте полёта (кг/м³): ";
    std::cin >> rho;
    std::cout << "Вводимое значение может быть только положительным числом. Введите скорость набегающего потока V (м/с): ";
    std::cin >> v;
    std::cout << "Вводимое значение может быть только положительным числом. Введите площадь крыла S (м²): ";
    std::cin >> s;
    std::cout << "Введите коэффициент коэффициент сопротивления: ";
    std::cin >> cd;
    L = 0.5 * rho * pow(v,2) * s * cd;
    std::cout << "Подъемная сила L = "<< L <<"" << std::endl;
    return 0;
}