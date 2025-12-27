#include <iostream>
#include <iomanip>

using namespace std;

class Engine {
private:
    double thrust;
    double fuelFlow;
    double fuel;

public:
    Engine(double t, double ff, double f) {
        thrust = t;
        fuelFlow = ff;
        fuel = f;
    }

    double getThrust() {
        if (fuel > 0.0) {
            return thrust;
        }
        return 0.0;
    }

    bool hasFuel() {
        return fuel > 0.0;
    }

    void burn(double dt) {
        fuel -= fuelFlow * dt;
        if (fuel < 0.0) {
            fuel = 0.0;
        }
    }
};

class NavigationSystem {
private:
    double altitude;
    double velocity;

public:
    NavigationSystem() {
        altitude = 0.0;
        velocity = 0.0;
    }

    void update(double thrust, double dt) {
        velocity += (thrust / 1000.0 - 9.8) * dt;
        altitude += velocity * dt;
    }

    double getAltitude() {
        return altitude;
    }

    double getVelocity() {
        return velocity;
    }
};

class ControlSystem {
private:
    double targetAltitude;

public:
    ControlSystem(double target) {
        targetAltitude = target;
    }

    double computeThrust(double altitude) {
        if (altitude < targetAltitude) {
            return 12000.0;
        }
        return 8000.0;
    }
};

class Rocket {
private:
    Engine engine;
    NavigationSystem navigation;
    ControlSystem control;

public:
    Rocket(double thrust, double fuelFlow, double fuel, double targetAlt)
        : engine(thrust, fuelFlow, fuel), control(targetAlt) {}

    void simulateStep(double dt) {
        double currentThrust = control.computeThrust(navigation.getAltitude());
        if (engine.hasFuel()) {
            engine.burn(dt);
            navigation.update(currentThrust, dt);
        }
    }

    void printStatus(double time) {
        cout << fixed << setprecision(2);
        cout << "t=" << time
             << "s altitude=" << navigation.getAltitude()
             << "m velocity=" << navigation.getVelocity()
             << "m/s" << endl;
    }
};

int main() {
    Rocket rocket(12000.0, 2.0, 50.0, 1000.0);

    double dt = 0.5;
    double time = 0.0;

    for (int i = 0; i < 10; i++) {
        rocket.simulateStep(dt);
        time += dt;
        rocket.printStatus(time);
    }

    return 0;
}
