#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <numeric>

class TrajectoryLogger {
private:
    struct Point {
        double x, y, z, speed, time;
    };
    std::vector<Point> points;
    std::string filename;

public:
    TrajectoryLogger(const std::string& fname) : filename(fname) {}

    void addPoint(double x, double y, double z, double speed, double time) {
        points.push_back({ x, y, z, speed, time });
    }

    bool saveToCSV() {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << "time,x,y,z,speed\n";
        for (const auto& p : points) {
            file << p.time << "," << p.x << "," << p.y << "," << p.z << "," << p.speed << "\n";
        }
        file.close();
        return true;
    }

    bool loadFromCSV() {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string header;
        std::getline(file, header);
        double t, x, y, z, s;
        char comma;
        points.clear();
        while (file >> t >> comma >> x >> comma >> y >> comma >> z >> comma >> s) {
            points.push_back({ x, y, z, s, t });
        }
        file.close();
        return true;
    }

    double calculateTotalDistance() {
        double dist = 0.0;
        for (size_t i = 1; i < points.size(); i++) {
            double dx = points[i].x - points[i - 1].x;
            double dy = points[i].y - points[i - 1].y;
            double dz = points[i].z - points[i - 1].z;
            dist += sqrt(dx * dx + dy * dy + dz * dz);
        }
        return dist;
    }

    double findMaxSpeed() {
        double maxSpeed = 0.0;
        for (const auto& p : points) {
            if (p.speed > maxSpeed) maxSpeed = p.speed;
        }
        return maxSpeed;
    }

    void printStatistics() {
        std::cout << "Total points: " << points.size() << "\n";
        std::cout << "Total distance: " << calculateTotalDistance() << "\n";
        std::cout << "Max speed: " << findMaxSpeed() << "\n";
    }
};

class TargetManager {
private:
    struct Target {
        int id;
        std::string name;
        double x, y, z, priority, distance;
    };
    std::vector<Target> targets;
    std::string filename = "targets.bd";

public:
    void addTarget(int id, const std::string& name, double x, double y, double z,
        double priority, double distance) {
        targets.push_back({ id, name, x, y, z, priority, distance });
    }

    bool removeTarget(int target_id) {
        auto it = std::remove_if(targets.begin(), targets.end(),
            [target_id](const Target& t) { return t.id == target_id; });
        if (it != targets.end()) {
            targets.erase(it, targets.end());
            return true;
        }
        return false;
    }

    void saveTargetsToFile() {
        std::ofstream file(filename);
        for (const auto& t : targets) {
            file << t.id << "," << t.name << "," << t.x << "," << t.y << "," << t.z << ","
                << t.priority << "," << t.distance << "\n";
        }
        file.close();
    }

    void loadTargetsFromFile() {
        std::ifstream file(filename);
        targets.clear();
        int id;
        std::string name;
        double x, y, z, pr, dist;
        char comma;
        while (file >> id >> comma && std::getline(file, name, ',') &&
            file >> x >> comma >> y >> comma >> z >> comma >> pr >> comma >> dist) {
            targets.push_back({ id, name, x, y, z, pr, dist });
        }
        file.close();
    }

    std::vector<Target> getHighPriorityTargets(double min_priority) {
        std::vector<Target> high;
        std::copy_if(targets.begin(), targets.end(), std::back_inserter(high),
            [min_priority](const Target& t) { return t.priority >= min_priority; });
        return high;
    }

    void sortByDistance() {
        std::sort(targets.begin(), targets.end(),
            [](const Target& a, const Target& b) { return a.distance < b.distance; });
    }
};

struct TelemetryData {
    double time, altitude, speed, heading, fuel;
};

class TelemetryLogger {
private:
    std::string baseFilename = "telemetry";
    int fileCounter = 1;
    const size_t maxEntries = 1000;
    std::vector<TelemetryData> buffer;

public:
    bool logData(double time, double altitude, double speed, double heading, double fuel) {
        buffer.push_back({ time, altitude, speed, heading, fuel });
        if (buffer.size() >= maxEntries) {
            rotateFileIfNeeded();
        }
        return true;
    }

    void rotateFileIfNeeded() {
        if (buffer.empty()) return;
        std::string filename = baseFilename + "_" + std::to_string(fileCounter++) + ".bin";
        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size() * sizeof(TelemetryData));
        file.close();
        buffer.clear();
    }

    std::vector<TelemetryData> readLogFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) return {};
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0, std::ios::beg);
        size_t count = size / sizeof(TelemetryData);
        std::vector<TelemetryData> data(count);
        file.read(reinterpret_cast<char*>(data.data()), size);
        file.close();
        return data;
    }

    void printLogSummary() {
        double totalTime = 0, avgAlt = 0, avgSpeed = 0;
        for (const auto& d : buffer) {
            totalTime += d.time;
            avgAlt += d.altitude;
            avgSpeed += d.speed;
        }
        std::cout << "Entries: " << buffer.size() << "\n";
        std::cout << "Avg Altitude: " << avgAlt / buffer.size() << "\n";
        std::cout << "Avg Speed: " << avgSpeed / buffer.size() << "\n";
    }
};

struct Waypoint {
    int id;
    double x, y, z, speed;
    std::string desc;
};

class WaypointManager {
private:
    std::vector<Waypoint> waypoints;
    int currentIndex = 0;

public:
    void addWaypoint(int id, double x, double y, double z, double speed, const std::string& desc) {
        waypoints.push_back({ id, x, y, z, speed, desc });
    }

    bool saveRoute() {
        std::ofstream file("waypoints.bd");
        for (const auto& w : waypoints) {
            file << w.id << "," << w.x << "," << w.y << "," << w.z << "," << w.speed << "," << w.desc << "\n";
        }
        file.close();
        return true;
    }

    bool loadRoute() {
        std::ifstream file("waypoints.bd");
        waypoints.clear();
        int id;
        double x, y, z, speed;
        std::string desc;
        char comma;
        while (file >> id >> comma >> x >> comma >> y >> comma >> z >> comma >> speed >> comma && std::getline(file, desc)) {
            waypoints.push_back({ id, x, y, z, speed, desc });
        }
        file.close();
        return !waypoints.empty();
    }

    double calculateTotalDistance() {
        double dist = 0.0;
        for (size_t i = 1; i < waypoints.size(); i++) {
            double dx = waypoints[i].x - waypoints[i - 1].x;
            double dy = waypoints[i].y - waypoints[i - 1].y;
            double dz = waypoints[i].z - waypoints[i - 1].z;
            dist += sqrt(dx * dx + dy * dy + dz * dz);
        }
        return dist;
    }

    Waypoint getNextWaypoint() {
        if (currentIndex < waypoints.size()) return waypoints[currentIndex];
        return {};
    }

    bool checkWaypointReached(double x, double y, double z) {
        if (currentIndex >= waypoints.size()) return false;
        auto& w = waypoints[currentIndex];
        double dist = sqrt(pow(x - w.x, 2) + pow(y - w.y, 2) + pow(z - w.z, 2));
        if (dist < 10.0) { // tolerance
            currentIndex++;
            return true;
        }
        return false;
    }
};

class DataValidator {
private:
    struct Rule {
        double min, max;
    };
    std::map<std::string, Rule> rules;
    std::vector<std::string> errors;

public:
    void addValidationRule(const std::string& field, double min, double max) {
        rules[field] = { min, max };
    }

    bool validateCoordinates(double x, double y, double z) {
        bool valid = true;
        if (rules.count("x") && (x < rules["x"].min || x > rules["x"].max)) {
            errors.push_back("Координата x вне диапазона");
            valid = false;
        }
        if (rules.count("z") && z > 5000) {
            errors.push_back("Высота z превышает максимум 5000");
            valid = false;
        }
        return valid;
    }

    bool validateSpeed(double speed) {
        if (rules.count("speed") && speed > 300) {
            errors.push_back("Скорость превышает максимум 300");
            return false;
        }
        return true;
    }

    bool validateAcceleration(double acceleration) {
        if (rules.count("acceleration") && acceleration > 20) {
            errors.push_back("Ускорение превышает максимум 20");
            return false;
        }
        return true;
    }

    void generateValidationReport() {
        std::ofstream file("validation_report.txt");
        file << "Отчет валидации:\n";
        for (const auto& err : errors) {
            file << err << "\n";
        }
        file << "Общий результат: " << getValidationScore() * 100 << "% данных валидны\n";
        file.close();
    }

    double getValidationScore() {
        return errors.empty() ? 1.0 : 0.0; // simplified
    }
};

class TelemetryFilter {
private:
    std::vector<std::vector<std::string>> data;

public:
    bool loadFromCSV(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::vector<std::string> row;
            std::string cell;
            while (std::getline(ss, cell, ',')) {
                row.push_back(cell);
            }
            data.push_back(row);
        }
        file.close();
        return true;
    }

    void filterData() {
        auto isValidAltitude = [](double alt) { return alt >= 0 && alt <= 20000; };
        auto isValidSpeed = [](double speed) { return speed >= 0 && speed <= 500; };

        std::vector<std::vector<std::string>> filtered;
        filtered.push_back(data[0]); // header
        for (size_t i = 1; i < data.size(); i++) {
            double alt = std::stod(data[i][1]);
            double speed = std::stod(data[i][2]);
            if (isValidAltitude(alt) && isValidSpeed(speed)) {
                filtered.push_back(data[i]);
            }
        }
        data = filtered;
    }

    bool saveToCSV(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& row : data) {
            for (size_t j = 0; j < row.size(); j++) {
                file << row[j];
                if (j < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
        return true;
    }

    void printFilteredStats() {
        std::cout << "Filtered rows: " << data.size() - 1 << "\n";
    }
};

class WaypointSorter {
private:
    struct Waypoint {
        int id;
        double x, y, z;
        std::string name;
        double distance;
    };
    std::vector<Waypoint> waypoints;

public:
    bool loadWaypoints(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        int id;
        double x, y, z;
        std::string name;
        char comma;
        while (file >> id >> comma >> x >> comma >> y >> comma >> z >> comma && std::getline(file, name)) {
            waypoints.push_back({ id, x, y, z, name, 0.0 });
        }
        file.close();
        return true;
    }

    void calculateDistances(double current_x, double current_y, double current_z) {
        for (auto& w : waypoints) {
            double dx = w.x - current_x;
            double dy = w.y - current_y;
            double dz = w.z - current_z;
            w.distance = sqrt(dx * dx + dy * dy + dz * dz);
        }
    }

    void sortByDistance() {
        std::sort(waypoints.begin(), waypoints.end(),
            [](const Waypoint& a, const Waypoint& b) { return a.distance < b.distance; });
    }

    void saveSortedWaypoints(const std::string& filename) {
        std::ofstream file(filename);
        for (const auto& w : waypoints) {
            file << w.id << "," << w.x << "," << w.y << "," << w.z << "," << w.name << "," << w.distance << "\n";
        }
        file.close();
    }
};

class FuelAnalyzer {
private:
    std::vector<double> time_data;
    std::vector<double> fuel_data;
    std::vector<double> rpm_data;

public:
    bool loadData(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string header;
        std::getline(file, header);
        double t, f, r;
        char comma;
        while (file >> t >> comma >> f >> comma >> r) {
            time_data.push_back(t);
            fuel_data.push_back(f);
            rpm_data.push_back(r);
        }
        file.close();
        return true;
    }

    void detectAnomalies() {
        double avg = calculateAverageConsumption();
        auto isAnomaly = [avg](double consumption) { return consumption > avg * 1.5; };
        for (size_t i = 0; i < fuel_data.size(); i++) {
            if (isAnomaly(fuel_data[i])) {
                std::cout << "Anomaly at time " << time_data[i] << ": " << fuel_data[i] << "\n";
            }
        }
    }

    double calculateAverageConsumption() {
        if (fuel_data.empty()) return 0.0;
        double sum = std::accumulate(fuel_data.begin(), fuel_data.end(), 0.0);
        return sum / fuel_data.size();
    }

    void generateReport(const std::string& filename) {
        std::ofstream file(filename);
        file << "Average fuel consumption: " << calculateAverageConsumption() << "\n";
        file << "Anomalies detected:\n";
        // логика вывода аномалий
        file.close();
    }
};

class Aircraft {
private:
    std::map<std::string, double> params;

public:
    bool loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string line;
        while (std::getline(file, line)) {
            size_t eq = line.find('=');
            if (eq != std::string::npos) {
                std::string key = line.substr(0, eq);
                double value = std::stod(line.substr(eq + 1));
                params[key] = value;
            }
        }
        file.close();
        return true;
    }
};

class Environment {
private:
    std::vector<std::tuple<double, double, double>> atmosphereTable; // alt, density, pressure

public:
    bool loadAtmosphereTable(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        std::string header;
        std::getline(file, header);
        double alt, dens, press;
        char comma;
        while (file >> alt >> comma >> dens >> comma >> press) {
            atmosphereTable.push_back({ alt, dens, press });
        }
        file.close();
        return true;
    }

    double getDensity(double altitude) {
        for (const auto& row : atmosphereTable) {
            if (std::get<0>(row) >= altitude) return std::get<1>(row);
        }
        return 1.225; // default sea level
    }
};

struct TrajectoryPoint {
    double time, velocity, altitude, distance, fuel;
};

class Trajectory {
private:
    std::vector<TrajectoryPoint> points;

public:
    bool saveToCSV(const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        file << "time,velocity,altitude,distance,fuel\n";
        for (const auto& p : points) {
            file << p.time << "," << p.velocity << "," << p.altitude << ","
                << p.distance << "," << p.fuel << "\n";
        }
        file.close();
        return true;
    }

    void generatePlotScript(const std::string& filename) {
        std::ofstream file(filename);
        file << "set terminal png\n";
        file << "set output 'trajectory_plot.png'\n";
        file << "plot 'trajectory.csv' using 1:3 with lines title 'Altitude'\n";
        file.close();
    }

    void saveReport(const std::string& filename) {
        double totalTime = 0, totalFuel = 0, avgVel = 0;
        if (!points.empty()) {
            totalTime = points.back().time;
            totalFuel = points.back().fuel;
            for (const auto& p : points) avgVel += p.velocity;
            avgVel /= points.size();
        }
        std::ofstream file(filename);
        file << "Total flight time: " << totalTime << " s\n";
        file << "Total fuel consumed: " << totalFuel << " kg\n";
        file << "Average velocity: " << avgVel << " m/s\n";
        file.close();
    }
};

int main()
{
    std::cout << "Hello World!\n";
}
