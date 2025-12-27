#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

struct TrajectoryPoint {
    double x;
    double y;
    double z;
    double speed;
    double time;
};

class TrajectoryLogger {
public:
    TrajectoryLogger(const std::string& filename) : filename_(filename) {}

    void addPoint(double x, double y, double z, double speed, double time) {
        TrajectoryPoint p;
        p.x = x;
        p.y = y;
        p.z = z;
        p.speed = speed;
        p.time = time;
        points_.push_back(p);
    }

    bool saveToCSV() {
        std::ofstream out(filename_.c_str());
        if (!out.is_open()) return false;

        out << "time,x,y,z,speed\n";
        out << std::fixed << std::setprecision(1);

        for (size_t i = 0; i < points_.size(); ++i) {
            out << points_[i].time << ","
                << points_[i].x << ","
                << points_[i].y << ","
                << points_[i].z << ","
                << points_[i].speed << "\n";
        }
        return true;
    }

    bool loadFromCSV() {
        std::ifstream in(filename_.c_str());
        if (!in.is_open()) return false;

        points_.clear();

        std::string line;
        if (!std::getline(in, line)) return false;

        while (std::getline(in, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> parts;

            while (std::getline(ss, token, ',')) {
                parts.push_back(token);
            }

            if (parts.size() != 5) continue;

            TrajectoryPoint p;
            p.time = std::atof(parts[0].c_str());
            p.x = std::atof(parts[1].c_str());
            p.y = std::atof(parts[2].c_str());
            p.z = std::atof(parts[3].c_str());
            p.speed = std::atof(parts[4].c_str());

            points_.push_back(p);
        }

        return true;
    }

    double calculateTotalDistance() {
        if (points_.size() < 2) return 0.0;

        double sum = 0.0;
        for (size_t i = 1; i < points_.size(); ++i) {
            double dx = points_[i].x - points_[i - 1].x;
            double dy = points_[i].y - points_[i - 1].y;
            double dz = points_[i].z - points_[i - 1].z;
            sum += std::sqrt(dx * dx + dy * dy + dz * dz);
        }
        return sum;
    }

    double findMaxSpeed() {
        if (points_.empty()) return 0.0;

        double mx = points_[0].speed;
        for (size_t i = 1; i < points_.size(); ++i) {
            if (points_[i].speed > mx) mx = points_[i].speed;
        }
        return mx;
    }

    void printStatistics() {
        std::cout << "points=" << points_.size() << "\n";
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "total_distance=" << calculateTotalDistance() << "\n";
        std::cout << std::fixed << std::setprecision(1);
        std::cout << "max_speed=" << findMaxSpeed() << "\n";
    }

private:
    std::string filename_;
    std::vector<TrajectoryPoint> points_;
};

int main() {
    TrajectoryLogger logger("trajectory.csv");

    logger.addPoint(10.5, 20.3, 100.0, 25.0, 0.0);
    logger.addPoint(15.2, 25.1, 105.0, 27.5, 1.5);
    logger.addPoint(20.8, 30.7, 110.0, 30.0, 3.0);

    if (!logger.saveToCSV()) {
        std::cout << "save error\n";
        return 1;
    }

    TrajectoryLogger loaded("trajectory.csv");
    if (!loaded.loadFromCSV()) {
        std::cout << "load error\n";
        return 1;
    }

    loaded.printStatistics();
    return 0;
}
