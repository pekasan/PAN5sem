#include <iostream>

using namespace std;

class Gyroscope {
private:
    double angularVelocity;
    bool calibrationRequired;

public:
    Gyroscope(double v, bool needCalib) {
        angularVelocity = v;
        calibrationRequired = needCalib;
    }

    void calibrate() {
        calibrationRequired = false;
    }

    void printStatus() {
        cout << "Angular velocity: " << angularVelocity << " deg/s | ";
        if (calibrationRequired) {
            cout << "Calibration required";
        } else {
            cout << "Calibration not required";
        }
        cout << endl;
    }
};

int main() {
    Gyroscope g1(120.5, true);

    g1.printStatus();
    g1.calibrate();
    g1.printStatus();

    return 0;
}
