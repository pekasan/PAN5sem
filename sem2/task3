#include <iostream>
using namespace std;

double accel_horizontal(double T, double D, double m) {
    return (T - D) / m;
}

double accel_vertical(double L, double m) {
    const double g = 9.81;
    return (L - m * g) / m;
}

int main() {
    double m, L, D, T;

    cout << "Enter mass m:\n";
    cin >> m;

    cout << "Enter lift force L:\n";
    cin >> L;

    cout << "Enter drag force D:\n";
    cin >> D;

    cout << "Enter thrust T:\n";
    cin >> T;

    double a = accel_horizontal(T, D, m);
    double ay = accel_vertical(L, m);

    cout << "Horizontal acceleration a = " << a << "\n";
    cout << "Vertical acceleration ay = " << ay << "\n";

    return 0;
}
