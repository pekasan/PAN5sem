#include <iostream>

using namespace std;

class NavigationSystem {
private:
    double x;
    double y;
    double velocityX;
    double velocityY;
    bool gpsAvailable;

public:
    NavigationSystem(double x0, double y0, double vx, double vy, bool gps) {
        x = x0;
        y = y0;
        velocityX = vx;
        velocityY = vy;
        gpsAvailable = gps;
    }

    void integratePosition(double dt) {
        x += velocityX * dt;
        y += velocityY * dt;
    }

    void correctGPS(double realX, double realY) {
        if (gpsAvailable) {
            x = (x + realX) / 2.0;
            y = (y + realY) / 2.0;
        }
    }

    void printPosition() {
        cout << "Corrected coordinates: (" << x << ", " << y << ")" << endl;
    }
};

int main() {
    NavigationSystem nav(0.0, 0.0, 100.0, 50.0, true);

    nav.integratePosition(1.0);
    nav.correctGPS(110.0, 60.0);
    nav.printPosition();

    return 0;
}
