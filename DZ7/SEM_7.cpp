#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <utility>

class Trajectory {
private:
    std::vector<double> t;
    std::vector<double> x;
public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        // Пропускаем заголовок "t,x"
        std::getline(file, line);
        double ti, xi;
        char comma;
        while (file >> ti >> comma >> xi) {
            t.push_back(ti);
            x.push_back(xi);
        }
        file.close();
        return true;
    }

    void computeVelocity() {
        std::vector<double> v;
        auto velLambda = [this](size_t i) {
            return (x[i + 1] - x[i]) / (t[i + 1] - t[i]);
            };
        for (size_t i = 0; i < t.size() - 1; i++) {
            v.push_back(velLambda(i));
        }
        v.push_back(v.back()); // дублируем последнее значение

        // Сохраняем результаты
        std::ofstream out("velocity.csv");
        out << "t,x,v\n";
        for (size_t i = 0; i < t.size(); i++) {
            out << t[i] << "," << x[i] << "," << v[i] << "\n";
        }
        out.close();
    }

    void plot() {
        FILE* gp = _popen("gnuplot -persist", "w");
        fprintf(gp, "set title 'Trajectory x(t)'\n");
        fprintf(gp, "set xlabel 'Time (s)'\n");
        fprintf(gp, "set ylabel 'Position (m)'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot 'velocity.csv' using 1:2 with linespoints title 'x(t)'\n");
        fflush(gp);
        _pclose(gp);
    }
};

class SensorData {
private:
    std::vector<double> t, h1, h2;
public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        std::getline(file, line); // заголовок
        double ti, hi1, hi2;
        char comma;
        while (file >> ti >> comma >> hi1 >> comma >> hi2) {
            t.push_back(ti);
            h1.push_back(hi1);
            h2.push_back(hi2);
        }
        file.close();
        return true;
    }

    void computeDifference() {
        auto diffLambda = [](double a, double b) { return std::abs(a - b); };
        std::ofstream out("diff.csv");
        out << "t,h1,h2,diff\n";
        for (size_t i = 0; i < t.size(); i++) {
            double delta = diffLambda(h1[i], h2[i]);
            out << t[i] << "," << h1[i] << "," << h2[i] << "," << delta << "\n";
        }
        out.close();
    }

    void plot() {
        FILE* gp = _popen("gnuplot -persist", "w");
        fprintf(gp, "set title 'Sensor Comparison'\n");
        fprintf(gp, "set xlabel 'Time (s)'\n");
        fprintf(gp, "set ylabel 'Height (m)'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot 'diff.csv' using 1:2 with lines title 'h1(t)', "
            "'diff.csv' using 1:3 with lines title 'h2(t)', "
            "'diff.csv' using 1:4 with lines title 'Δh(t)'\n");
        fflush(gp);
        _pclose(gp);
    }
};

class Navigator {
private:
    std::vector<double> t, x, y;
public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        std::getline(file, line); // заголовок
        double ti, xi, yi;
        char comma;
        while (file >> ti >> comma >> xi >> comma >> yi) {
            t.push_back(ti);
            x.push_back(xi);
            y.push_back(yi);
        }
        file.close();
        return true;
    }

    void computeVelocity() {
        std::vector<double> vx, vy, v;
        for (size_t i = 0; i < t.size() - 1; i++) {
            vx.push_back((x[i + 1] - x[i]) / (t[i + 1] - t[i]));
            vy.push_back((y[i + 1] - y[i]) / (t[i + 1] - t[i]));
        }
        // Последнее значение дублируем
        vx.push_back(vx.back());
        vy.push_back(vy.back());

        // Модуль скорости
        auto speedLambda = [](double vx, double vy) { return std::sqrt(vx * vx + vy * vy); };
        for (size_t i = 0; i < t.size(); i++) {
            v.push_back(speedLambda(vx[i], vy[i]));
        }

        // Сохранение
        std::ofstream out("velocity_nav.csv");
        out << "t,v\n";
        for (size_t i = 0; i < t.size(); i++) {
            out << t[i] << "," << v[i] << "\n";
        }
        out.close();
    }

    void plot() {
        FILE* gp = _popen("gnuplot -persist", "w");
        fprintf(gp, "set title 'Speed v(t)'\n");
        fprintf(gp, "set xlabel 'Time (s)'\n");
        fprintf(gp, "set ylabel 'Speed (m/s)'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot 'velocity_nav.csv' using 1:2 with linespoints title 'v(t)'\n");
        fflush(gp);
        _pclose(gp);
    }
};

class MotionAnalyzer {
private:
    std::vector<double> t, x, v, a;
public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        std::getline(file, line); // заголовок
        double ti, xi;
        char comma;
        while (file >> ti >> comma >> xi) {
            t.push_back(ti);
            x.push_back(xi);
        }
        file.close();
        return true;
    }

    void computeVelocity() {
        v.resize(t.size());
        auto velLambda = [this](size_t i) {
            return (x[i + 1] - x[i]) / (t[i + 1] - t[i]);
            };
        for (size_t i = 0; i < t.size() - 1; i++) {
            v[i] = velLambda(i);
        }
        v.back() = v[v.size() - 2]; // последнее = предпоследнее
    }

    void computeAcceleration() {
        a.resize(t.size());
        auto accLambda = [this](size_t i) {
            return (v[i + 1] - v[i]) / (t[i + 1] - t[i]);
            };
        for (size_t i = 0; i < t.size() - 1; i++) {
            a[i] = accLambda(i);
        }
        a.back() = a[a.size() - 2];
    }

    void saveResults(const std::string& filename) {
        std::ofstream out(filename);
        out << "t,x,v,a\n";
        for (size_t i = 0; i < t.size(); i++) {
            out << t[i] << "," << x[i] << "," << v[i] << "," << a[i] << "\n";
        }
        out.close();
    }

    void plot() {
        FILE* gp = _popen("gnuplot -persist", "w");
        fprintf(gp, "set title 'Motion Analysis'\n");
        fprintf(gp, "set xlabel 'Time (s)'\n");
        fprintf(gp, "set ylabel 'Value'\n");
        fprintf(gp, "set grid\n");
        fprintf(gp, "plot 'motion_processed.csv' using 1:3 with lines title 'v(t)', "
            "'motion_processed.csv' using 1:4 with lines title 'a(t)'\n");
        fflush(gp);
        _pclose(gp);
    }
};

int main()
{
    int tusk_num = 1;
    switch (tusk_num) {
        case 1: {
            Trajectory traj;
            if (!traj.loadFromFile("traj.csv")) {
                std::cerr << "Ошибка загрузки файла\n";
                return 1;
            }
            traj.computeVelocity();
            traj.plot();
            break;
        }
        case 2: {
            SensorData sensors;
            if (!sensors.loadFromFile("sensors.csv")) {
                std::cerr << "Ошибка загрузки файла\n";
                return 1;
            }
            sensors.computeDifference();
            sensors.plot();
            break;
        }
        case 3: {
            std::vector<std::pair<double, double>> data;
            std::ifstream in("altitude.csv");
            if (!in.is_open()) {
                std::cerr << "Ошибка открытия altitude.csv\n";
                return 1;
            }
            std::string line;
            std::getline(in, line); // заголовок
            double t, H;
            char comma;
            while (in >> t >> comma >> H) {
                data.push_back({ t, H });
            }
            in.close();

            // Фильтрация
            auto filterLambda = [](const std::pair<double, double>& p) {
                return p.second > 900 && p.second < 1100;
                };
            std::vector<std::pair<double, double>> filtered;
            std::copy_if(data.begin(), data.end(), std::back_inserter(filtered), filterLambda);

            // Запись отфильтрованных данных
            std::ofstream out("filtered.csv");
            out << "t,H\n";
            for (const auto& p : filtered) {
                out << p.first << "," << p.second << "\n";
            }
            out.close();

            // GNUPlot
            FILE* gp = _popen("gnuplot -persist", "w");
            fprintf(gp, "set title 'Altitude Filtering'\n");
            fprintf(gp, "set xlabel 'Time (s)'\n");
            fprintf(gp, "set ylabel 'Altitude (m)'\n");
            fprintf(gp, "set grid\n");
            fprintf(gp, "plot 'altitude.csv' using 1:2 with points title 'Original', "
                "'filtered.csv' using 1:2 with lines title 'Filtered'\n");
            fflush(gp);
            _pclose(gp);
            break;
        }
        case 4: {
            Navigator nav;
            if (!nav.loadFromFile("navigation.csv")) {
                std::cerr << "Ошибка загрузки файла\n";
                return 1;
            }
            nav.computeVelocity();
            nav.plot();
            break;
        }
        case 5: {
            MotionAnalyzer ma;
            if (!ma.loadFromFile("motion.csv")) {
                std::cerr << "Ошибка загрузки файла\n";
                return 1;
            }
            ma.computeVelocity();
            ma.computeAcceleration();
            ma.saveResults("motion_processed.csv");
            ma.plot();
            break;
        }
        default: {
            break;
        }
    }
}
