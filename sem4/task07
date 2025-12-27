#include <iostream>
#include <iomanip>

using namespace std;

class FlightComputer {
private:
    double altitude;
    double velocity;
    double fuel;
    double thrust;
    double time;

public:
    FlightComputer(double a, double v, double f, double t) {
        altitude = a;
        velocity = v;
        fuel = f;
        thrust = t;
        time = 0.0;
    }

    void simulateStep(double dt) {
        velocity += (thrust / 1000.0 - 9.8) * dt;
        altitude += velocity * dt;
        fuel -= dt * 2.0;
        time += dt;
    }

    void printStatus() {
        cout << fixed << setprecision(1);
        cout << "t=" << time << "s: altitude=" << altitude
             << "m, velocity=" << velocity
             << "m/s, fuel=" << fuel << endl;
    }
};

int main() {
    FlightComputer fc(0.0, 0.0, 100.0, 12000.0);

    for (int i = 0; i < 5; i++) {
        fc.simulateStep(0.5);
        fc.printStatus();
    }

    return 0;
}
