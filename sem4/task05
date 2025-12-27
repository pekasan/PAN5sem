#include <iostream>

using namespace std;

class Autopilot {
private:
    double courseAngle;
    double altitude;

public:
    Autopilot(double c, double a) {
        courseAngle = c;
        altitude = a;
    }

    void changeCourse(double delta) {
        courseAngle += delta;
    }

    void changeAltitude(double delta) {
        altitude += delta;
    }

    void printStatus() {
        cout << "Course: " << courseAngle << " deg, altitude: " << altitude << " m" << endl;
    }
};

int main() {
    Autopilot a1(90, 1000);

    a1.changeCourse(10);
    a1.changeAltitude(500);
    a1.printStatus();

    return 0;
}
