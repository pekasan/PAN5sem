#include <iostream>
#include <iomanip>

using namespace std;

class Engine {
private:
    double thrust;
    double fuelFlow;

public:
    Engine(double T, double F) {
        thrust = T;
        fuelFlow = F;
    }

    double getSpecificImpulse() {
        return thrust / (fuelFlow * 9.81);
    }

    void printInfo() {
        cout << fixed << setprecision(2);
        cout << "Thrust: " << thrust << " N | "
             << "Fuel flow: " << fuelFlow << " kg/s | "
             << "Specific impulse: " << getSpecificImpulse() << " s" << endl;
    }
};

int main() {
    Engine e1(5000, 2.5);
    e1.printInfo();

    return 0;
}
