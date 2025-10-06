#include <iostream> // input-output library
#include <cmath> // include math
#include <string> 
#include <iomanip>
#include <vector>

const double g = 9.81;
const double RHO = 1.225; // плотность воздуха
const double VELOCITY = 100.0; // скорость полета (м/с)

// Структура для хранения параметров самолета
struct Aircraft {
    std::string name;        // название самолета
    double mass;        // масса (кг)
    double wing_area;   // площадь крыла (м²)
    double thrust;      // тяга (Н)
    double cl;          // коэффициент подъемной силы C_L
    double cd;          // коэффициент сопротивления C_D
    double lift;        // подъемная сила (Н) - вычисляемое
    double drag;        // сопротивление (Н) - вычисляемое
    double acceleration; // ускорение (м/с²) - вычисляемое
};

// Функция для расчета подъемной силы
double calculateLift(double rho, double v, double s, double cl) {
    return 0.5 * rho * pow(v, 2) * s * cl;
}

// Функция для расчета сопротивления
double calculateDrag(double rho, double v, double s, double cd) {
    return 0.5 * rho * pow(v, 2) * s * cd;
}

// Функция для расчета ускорения
double calculateAcceleration(double thrust, double drag, double mass) {
    return (thrust - drag) / mass;
}

int main() {
    int numAircraft;
    
    std::cout << "=== ИНТЕРАКТИВНЫЙ РАСЧЕТ АЭРОДИНАМИЧЕСКИХ ХАРАКТЕРИСТИК ===" << std::endl;
    std::cout << std::endl;
    
    // Ввод количества самолетов
    std::cout << "Введите количество самолетов: ";
    std::cin >> numAircraft;
    
    if (numAircraft <= 0) {
        std::cout << "Ошибка: количество самолетов должно быть положительным!" << std::endl;
        return 1;
    }
    
    // Создаем вектор для хранения самолетов
    std::vector<Aircraft> aircrafts(numAircraft);
    
    // Ввод данных для каждого самолета
    for (int i = 0; i < numAircraft; i++) {
        std::cout << std::endl;
        std::cout << "=== САМОЛЕТ " << (i + 1) << " ===" << std::endl;
        
        std::cout << "Введите название самолета: ";
        std::cin.ignore(); // очищаем буфер
        std::getline(std::cin, aircrafts[i].name);
        
        std::cout << "Масса (кг): ";
        std::cin >> aircrafts[i].mass;
        
        std::cout << "Площадь крыла (м²): ";
        std::cin >> aircrafts[i].wing_area;
        
        std::cout << "Тяга (Н): ";
        std::cin >> aircrafts[i].thrust;
        
        std::cout << "Коэффициент подъемной силы C_L: ";
        std::cin >> aircrafts[i].cl;
        
        std::cout << "Коэффициент сопротивления C_D: ";
        std::cin >> aircrafts[i].cd;
        
        // Проверка корректности введенных данных
        if (aircrafts[i].mass <= 0 || aircrafts[i].wing_area <= 0 || 
            aircrafts[i].thrust < 0 || aircrafts[i].cl <= 0 || aircrafts[i].cd <= 0) {
            std::cout << "Ошибка: все параметры должны быть положительными!" << std::endl;
            return 1;
        }
    }
    
    // Вычисление характеристик для каждого самолета
    for (int i = 0; i < numAircraft; i++) {
        aircrafts[i].lift = calculateLift(RHO, VELOCITY, aircrafts[i].wing_area, aircrafts[i].cl);
        aircrafts[i].drag = calculateDrag(RHO, VELOCITY, aircrafts[i].wing_area, aircrafts[i].cd);
        aircrafts[i].acceleration = calculateAcceleration(aircrafts[i].thrust, aircrafts[i].drag, aircrafts[i].mass);
    }
    
    // Поиск самолета с наибольшим ускорением
    int bestIndex = 0;
    double maxAcceleration = aircrafts[0].acceleration;
    
    for (int i = 1; i < numAircraft; i++) {
        if (aircrafts[i].acceleration > maxAcceleration) {
            maxAcceleration = aircrafts[i].acceleration;
            bestIndex = i;
        }
    }
    
    // Вывод результатов
    std::cout << std::endl;
    std::cout << "=== РЕЗУЛЬТАТЫ РАСЧЕТОВ ===" << std::endl;
    std::cout << "Скорость полета: " << VELOCITY << " м/с" << std::endl;
    std::cout << "Плотность воздуха: " << RHO << " кг/м³" << std::endl;
    std::cout << std::endl;
    
    // Таблица с результатами
    std::cout << std::string(100, '-') << std::endl;
    std::cout << std::setw(1) << "Самолет" 
              << std::setw(25) << "Масса" 
              << std::setw(25) << "Подъемная" 
              << std::setw(25) << "Сопротивл."
              << std::setw(15) << "Тяга"
              << std::setw(25) << "Ускорение" << std::endl;
    std::cout << std::setw(20) << "" 
              << std::setw(8) << "(кг)" 
              << std::setw(15) << "(кН)" 
              << std::setw(18) << "(кН)"
              << std::setw(20) << "(кН)"
              << std::setw(15) << "(м/с²)" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    std::cout << std::fixed << std::setprecision(3);
    
    for (int i = 0; i < numAircraft; i++) {
        std::cout << std::setw(1) << aircrafts[i].name
                  << std::setw(25) << aircrafts[i].mass
                  << std::setw(15) << aircrafts[i].lift / 1000
                  << std::setw(15) << aircrafts[i].drag / 1000
                  << std::setw(15) << aircrafts[i].thrust / 1000
                  << std::setw(15) << aircrafts[i].acceleration << std::endl;
    }
    
    std::cout << std::string(100, '-') << std::endl;
    
    // Вывод победителя
    std::cout << std::endl;
    std::cout << "=== ЛИДЕР ===" << std::endl;
    std::cout << "Самолет с наибольшим ускорением: " << aircrafts[bestIndex].name << std::endl;
    std::cout << "Ускорение: " << aircrafts[bestIndex].acceleration << " м/с²" << std::endl;
    
    return 0;
}