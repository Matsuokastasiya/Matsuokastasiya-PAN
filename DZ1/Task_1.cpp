#include <iostream> // input-output library
#include <cmath> // iclude math
//using namespace std; // cout without std::

int main(){
    float rho;
    float v;
    float s;
    float cl;
    float L;
    std::cout << "Вводимое значение может быть только положительным числом. Введите плотность воздуха r на высоте полёта (кг/м³): ";
    std::cin >> rho;
    std::cout << "Вводимое значение может быть только положительным числом. Введите скорость набегающего потока V (м/с): ";
    std::cin >> v;
    std::cout << "Вводимое значение может быть только положительным числом. Введите площадь крыла S (м²): ";
    std::cin >> s;
    std::cout << "Введите коэффициент подъёмной силы: ";
    std::cin >> cl;
    L = 0.5 * rho * pow(v,2) * s * cl;
    std::cout << "Подъемная сила L = "<< L <<"" << std::endl;
    return 0;
}