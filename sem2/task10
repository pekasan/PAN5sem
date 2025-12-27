#include <iostream>
#include <cmath>
using namespace std;

int main() {
    const double g = 9.81;

    double m, S, V, rho, CL, h;
    double Tmin, Tmax, dT;

    cout << "Enter m:\n";
    cin >> m;

    cout << "Enter S:\n";
    cin >> S;

    cout << "Enter V:\n";
    cin >> V;

    cout << "Enter rho:\n";
    cin >> rho;

    cout << "Enter CL:\n";
    cin >> CL;

    cout << "Enter height h:\n";
    cin >> h;

    cout << "Enter Tmin:\n";
    cin >> Tmin;

    cout << "Enter Tmax:\n";
    cin >> Tmax;

    cout << "Enter dT:\n";
    cin >> dT;

    double L = 0.5 * rho * V * V * S * CL;

    double bestT = 0;
    double bestTime = -1;

    for (double T = Tmin; T <= Tmax; T += dT) {
        double ay = (L + T - m * g) / m;

        if (ay > 0 && h > 0) {
            double t = sqrt(2.0 * h / ay);

            if (bestTime < 0 || t < bestTime) {
                bestTime = t;
                bestT = T;
            }
        }
    }

    if (bestTime < 0) {
        cout << "cannot climb\n";
    } else {
        cout << "Best T = " << bestT << "\n";
        cout << "Min time t = " << bestTime << "\n";
    }

    return 0;
}
