#include <iostream>
#include <iomanip>

using namespace std;

class RocketStage {
private:
    double thrust;
    double burnTime;
    double mass;

public:
    RocketStage(double T, double t, double m) {
        thrust = T;
        burnTime = t;
        mass = m;
    }

    double getDeltaV() {
        return (thrust * burnTime) / mass;
    }

    void printInfo() {
        cout << fixed << setprecision(2);
        cout << "Thrust: " << thrust
             << " N | Burn time: " << burnTime
             << " s | Mass: " << mass
             << " kg | Delta V: " << getDeltaV()
             << " m/s" << endl;
    }
};

int main() {
    RocketStage s1(5000, 10, 1000);
    RocketStage s2(7000, 8, 900);
    RocketStage s3(9000, 6, 800);

    double totalV = 0.0;

    s1.printInfo();
    totalV += s1.getDeltaV();

    s2.printInfo();
    totalV += s2.getDeltaV();

    s3.printInfo();
    totalV += s3.getDeltaV();

    cout << fixed << setprecision(2);
    cout << "Total rocket velocity: " << totalV << " m/s" << endl;

    return 0;
}
