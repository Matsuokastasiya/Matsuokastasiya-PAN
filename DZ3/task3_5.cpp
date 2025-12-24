#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <array>
#include <fstream> // для сохранения в файл

// Структура для хранения 3D точки
struct Point3D {
    double x, y, z;
    
    // Конструктор по умолчанию
    Point3D() : x(0.0), y(0.0), z(0.0) {}
    
    // Конструктор с параметрами
    Point3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
    
    // Метод для вычисления расстояния от начала координат
    double magnitude() const {
        return sqrt(x*x + y*y + z*z);
    }
    
    // Перегрузка оператора сложения с другой точкой
    Point3D operator+(const Point3D& other) const {
        return Point3D(x + other.x, y + other.y, z + other.z);
    }
    
    // Перегрузка оператора умножения на скаляр
    Point3D operator*(double scalar) const {
        return Point3D(x * scalar, y * scalar, z * scalar);
    }
    
    // Метод для добавления скорости (смещения)
    Point3D addVelocity(const Point3D& velocity, double dt) const {
        return Point3D(x + velocity.x * dt, 
                      y + velocity.y * dt, 
                      z + velocity.z * dt);
    }
};

// Используем Point3D и для представления скорости (более простой подход)
// Velocity3D теперь не нужна как отдельная структура

int main() {
    // Вариант 1: Простое движение без ускорения
    std::cout << "=== ВАРИАНТ 1: Простое движение без ускорения ===" << std::endl;
    
    // Начальные параметры
    const double vx0 = 30.0;   // начальная скорость по X (м/с)
    const double vy0 = 20.0;   // начальная скорость по Y (м/с)
    const double vz0 = 40.0;   // начальная скорость по Z (м/с)
    const double dt = 0.1;     // шаг времени (с)
    const double t_max = 5.0;  // время моделирования (с)
    
    // Начальные координаты и скорость
    Point3D position(0.0, 0.0, 0.0);
    Point3D velocity(vx0, vy0, vz0);
    
    // Создание вектора для хранения траектории
    std::vector<Point3D> trajectory1;
    std::vector<double> times1;
    
    // Резервирование памяти
    int estimated_points = static_cast<int>(t_max / dt) + 1;
    trajectory1.reserve(estimated_points);
    times1.reserve(estimated_points);
    
    // Моделирование движения без ускорения
    for (double t = 0.0; t <= t_max; t += dt) {
        // Обновление координат (без ускорения)
        position = Point3D(vx0 * t, vy0 * t, vz0 * t);
        
        // Сохранение точки и времени
        trajectory1.push_back(position);
        times1.push_back(t);
    }
    
    // Вывод таблицы для варианта 1
    std::cout << "\nТаблица траектории (первые 10 точек):" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    std::cout << std::setw(10) << "Время (с)" 
              << std::setw(15) << "X (м)" 
              << std::setw(15) << "Y (м)" 
              << std::setw(15) << "Z (м)" 
              << std::setw(15) << "|R| (м)" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    size_t points_to_show = std::min((size_t)10, trajectory1.size());
    for (size_t i = 0; i < points_to_show; ++i) {
        double magnitude = trajectory1[i].magnitude();
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << times1[i]
                  << std::setw(15) << trajectory1[i].x
                  << std::setw(15) << trajectory1[i].y
                  << std::setw(15) << trajectory1[i].z
                  << std::setw(15) << magnitude << std::endl;
    }
    
    if (trajectory1.size() > 10) {
        std::cout << "..." << std::endl;
        double last_magnitude = trajectory1.back().magnitude();
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << times1.back()
                  << std::setw(15) << trajectory1.back().x
                  << std::setw(15) << trajectory1.back().y
                  << std::setw(15) << trajectory1.back().z
                  << std::setw(15) << last_magnitude << std::endl;
    }
    
    std::cout << std::string(60, '-') << std::endl;
    
    // Проверка корректности
    std::cout << "\nПроверка корректности (без ускорений):" << std::endl;
    std::cout << "Теоретические значения при t = " << t_max << " с:" << std::endl;
    std::cout << "x_теор = vx0 * t = " << vx0 << " * " << t_max << " = " << vx0 * t_max << " м" << std::endl;
    std::cout << "y_теор = vy0 * t = " << vy0 << " * " << t_max << " = " << vy0 * t_max << " м" << std::endl;
    std::cout << "z_теор = vz0 * t = " << vz0 << " * " << t_max << " = " << vz0 * t_max << " м" << std::endl;
    std::cout << "Фактические значения:" << std::endl;
    std::cout << "x_факт = " << trajectory1.back().x << " м" << std::endl;
    std::cout << "y_факт = " << trajectory1.back().y << " м" << std::endl;
    std::cout << "z_факт = " << trajectory1.back().z << " м" << std::endl;
    
    // --- ВАРИАНТ 2: Движение с ускорением (гравитация по оси Z) ---
    std::cout << "\n\n=== ВАРИАНТ 2: Движение с гравитацией по оси Z ===" << std::endl;
    
    const double g = 9.8;  // ускорение свободного падения (м/с²)
    
    // Начальные параметры
    position = Point3D(0.0, 0.0, 0.0);
    velocity = Point3D(vx0, vy0, vz0);
    Point3D acceleration(0.0, 0.0, -g);  // ускорение только по оси Z
    
    // Создание векторов для хранения
    std::vector<Point3D> trajectory2;
    std::vector<Point3D> velocities;
    std::vector<double> times2;
    
    trajectory2.reserve(estimated_points);
    velocities.reserve(estimated_points);
    times2.reserve(estimated_points);
    
    // Моделирование движения с ускорением
    for (double t = 0.0; t <= t_max; t += dt) {
        // Сохранение текущего состояния
        trajectory2.push_back(position);
        velocities.push_back(velocity);
        times2.push_back(t);
        
        // Обновление скорости
        velocity = velocity + acceleration * dt;
        
        // Обновление позиции
        position = position.addVelocity(velocity, dt);
    }
    
    // Вывод таблицы для варианта 2
    std::cout << "\nТаблица траектории с гравитацией (первые 10 точек):" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    std::cout << std::setw(10) << "Время (с)" 
              << std::setw(15) << "X (м)" 
              << std::setw(15) << "Y (м)" 
              << std::setw(15) << "Z (м)" 
              << std::setw(15) << "Vx (м/с)"
              << std::setw(15) << "Vy (м/с)"
              << std::setw(15) << "Vz (м/с)" << std::endl;
    std::cout << std::string(90, '-') << std::endl;
    
    points_to_show = std::min((size_t)10, trajectory2.size());
    for (size_t i = 0; i < points_to_show; ++i) {
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << times2[i]
                  << std::setw(15) << trajectory2[i].x
                  << std::setw(15) << trajectory2[i].y
                  << std::setw(15) << trajectory2[i].z
                  << std::setw(15) << velocities[i].x
                  << std::setw(15) << velocities[i].y
                  << std::setw(15) << velocities[i].z << std::endl;
    }
    
    // --- ВАРИАНТ 3: Использование std::vector<std::array<double, 3>> ---
    std::cout << "\n\n=== ВАРИАНТ 3: Использование std::vector<std::array<double, 3>> ===" << std::endl;
    
    std::vector<std::array<double, 3>> trajectory3;  // [x, y, z]
    std::vector<std::array<double, 3>> velocities3;  // [vx, vy, vz]
    std::vector<double> times3;
    
    // Начальные значения
    double x3 = 0.0, y3 = 0.0, z3 = 0.0;
    double vx3 = vx0, vy3 = vy0, vz3 = vz0;
    
    // Моделирование с сопротивлением воздуха (упрощенная модель)
    const double drag_coefficient = 0.1;  // коэффициент сопротивления
    double speed, drag_force;
    
    for (double t = 0.0; t <= t_max; t += dt) {
        // Сохранение координат и скорости
        trajectory3.push_back({x3, y3, z3});
        velocities3.push_back({vx3, vy3, vz3});
        times3.push_back(t);
        
        // Вычисление скорости для сопротивления воздуха
        speed = sqrt(vx3*vx3 + vy3*vy3 + vz3*vz3);
        if (speed > 0) {
            drag_force = drag_coefficient * speed * speed;
            
            // Ускорение от сопротивления воздуха (противоположно скорости)
            double ax_drag = -drag_force * (vx3 / speed);
            double ay_drag = -drag_force * (vy3 / speed);
            double az_drag = -drag_force * (vz3 / speed) - g;  // гравитация + сопротивление
            
            // Обновление скоростей
            vx3 += ax_drag * dt;
            vy3 += ay_drag * dt;
            vz3 += az_drag * dt;
        } else {
            // Если скорость близка к нулю
            vz3 -= g * dt;  // только гравитация
        }
        
        // Обновление позиций
        x3 += vx3 * dt;
        y3 += vy3 * dt;
        z3 += vz3 * dt;
    }
    
    // Вывод статистики для всех вариантов
    std::cout << "\n=== СТАТИСТИКА ПО ВСЕМ ВАРИАНТАМ ===" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    std::cout << std::setw(20) << "Параметр" 
              << std::setw(15) << "Вариант 1" 
              << std::setw(15) << "Вариант 2" 
              << std::setw(15) << "Вариант 3" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    std::cout << std::fixed << std::setprecision(2)
              << std::setw(20) << "Количество точек"
              << std::setw(15) << trajectory1.size()
              << std::setw(15) << trajectory2.size()
              << std::setw(15) << trajectory3.size() << std::endl;
    
    std::cout << std::setw(20) << "Конечная X (м)"
              << std::setw(15) << trajectory1.back().x
              << std::setw(15) << trajectory2.back().x
              << std::setw(15) << trajectory3.back()[0] << std::endl;
    
    std::cout << std::setw(20) << "Конечная Y (м)"
              << std::setw(15) << trajectory1.back().y
              << std::setw(15) << trajectory2.back().y
              << std::setw(15) << trajectory3.back()[1] << std::endl;
    
    std::cout << std::setw(20) << "Конечная Z (м)"
              << std::setw(15) << trajectory1.back().z
              << std::setw(15) << trajectory2.back().z
              << std::setw(15) << trajectory3.back()[2] << std::endl;
    
    // Расчет расстояния от начала координат
    double dist1 = trajectory1.back().magnitude();
    double dist2 = trajectory2.back().magnitude();
    double dist3 = sqrt(trajectory3.back()[0]*trajectory3.back()[0] + 
                       trajectory3.back()[1]*trajectory3.back()[1] + 
                       trajectory3.back()[2]*trajectory3.back()[2]);
    
    std::cout << std::setw(20) << "Расстояние |R| (м)"
              << std::setw(15) << dist1
              << std::setw(15) << dist2
              << std::setw(15) << dist3 << std::endl;
    
    std::cout << std::string(70, '-') << std::endl;
    
    // --- Сохранение данных в файл для построения графиков ---
    std::cout << "\n=== СОХРАНЕНИЕ ДАННЫХ В ФАЙЛ ===" << std::endl;
    
    // Сохранение варианта 2 в файл
    std::ofstream outfile("trajectory_data.csv");
    if (outfile.is_open()) {
        outfile << "Time,X,Y,Z,Vx,Vy,Vz\n";
        for (size_t i = 0; i < trajectory2.size(); ++i) {
            outfile << std::fixed << std::setprecision(4)
                    << times2[i] << ","
                    << trajectory2[i].x << ","
                    << trajectory2[i].y << ","
                    << trajectory2[i].z << ","
                    << velocities[i].x << ","
                    << velocities[i].y << ","
                    << velocities[i].z << "\n";
        }
        outfile.close();
        std::cout << "Данные сохранены в файл: trajectory_data.csv" << std::endl;
        std::cout << "Можно использовать для построения графиков в Excel или Python" << std::endl;
    } else {
        std::cout << "Ошибка при сохранении данных в файл" << std::endl;
    }
    
    // --- Дополнительные расчеты ---
    std::cout << "\n=== ДОПОЛНИТЕЛЬНЫЕ РАСЧЕТЫ ===" << std::endl;
    
    // Расчет средней скорости для каждого варианта
    double total_distance1 = 0.0, total_distance2 = 0.0, total_distance3 = 0.0;
    
    for (size_t i = 1; i < trajectory1.size(); ++i) {
        double dx = trajectory1[i].x - trajectory1[i-1].x;
        double dy = trajectory1[i].y - trajectory1[i-1].y;
        double dz = trajectory1[i].z - trajectory1[i-1].z;
        total_distance1 += sqrt(dx*dx + dy*dy + dz*dz);
    }
    
    double avg_speed1 = total_distance1 / t_max;
    std::cout << "Вариант 1 - Средняя скорость: " << avg_speed1 << " м/с" << std::endl;
    
    // Анализ траектории
    std::cout << "\nАнализ траектории (Вариант 2):" << std::endl;
    
    // Нахождение максимальной высоты
    double max_z = trajectory2[0].z;
    double time_at_max_z = times2[0];
    for (size_t i = 1; i < trajectory2.size(); ++i) {
        if (trajectory2[i].z > max_z) {
            max_z = trajectory2[i].z;
            time_at_max_z = times2[i];
        }
    }
    
    std::cout << "Максимальная высота: " << max_z << " м при t = " << time_at_max_z << " с" << std::endl;
    
    // Нахождение момента "падения" (когда Z становится отрицательным)
    for (size_t i = 0; i < trajectory2.size(); ++i) {
        if (trajectory2[i].z < 0) {
            std::cout << "Ракета достигла земли при t ≈ " << times2[i] << " с" << std::endl;
            break;
        }
    }
    
    // Теоретический расчет максимальной высоты
    double theoretical_max_z = (vz0 * vz0) / (2 * g);
    double theoretical_time_to_max = vz0 / g;
    std::cout << "\nТеоретические значения:" << std::endl;
    std::cout << "Максимальная высота: " << theoretical_max_z << " м" << std::endl;
    std::cout << "Время достижения макс. высоты: " << theoretical_time_to_max << " с" << std::endl;
    
    // Сравнение с фактическими значениями
    std::cout << "\nСравнение с фактическими значениями:" << std::endl;
    std::cout << "Высота: теория=" << theoretical_max_z << " м, факт=" << max_z 
              << " м, разница=" << fabs(theoretical_max_z - max_z) << " м" << std::endl;
    std::cout << "Время: теория=" << theoretical_time_to_max << " с, факт=" << time_at_max_z 
              << " с, разница=" << fabs(theoretical_time_to_max - time_at_max_z) << " с" << std::endl;
    
    return 0;
}