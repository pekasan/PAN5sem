#include <iostream>
#include <cmath>
using namespace std;

struct Plane {
    double m, S, T, CL, CD;
    double L, D, a, ay, t;
};

int main() {
    const double g = 9.81;

    double V, rho, h;
    cout << "Enter V:\n";
    cin >> V;

    cout << "Enter rho:\n";
    cin >> rho;

    cout << "Enter h:\n";
    cin >> h;

    Plane p[3];

    for (int i = 0; i < 3; i++) {
        cout << "Plane " << (i + 1) << "\n";

        cout << "Enter m:\n";
        cin >> p[i].m;

        cout << "Enter S:\n";
        cin >> p[i].S;

        cout << "Enter T:\n";
        cin >> p[i].T;

        cout << "Enter CL:\n";
        cin >> p[i].CL;

        cout << "Enter CD:\n";
        cin >> p[i].CD;

        p[i].L  = 0.5 * rho * V * V * p[i].S * p[i].CL;
        p[i].D  = 0.5 * rho * V * V * p[i].S * p[i].CD;
        p[i].a  = (p[i].T - p[i].D) / p[i].m;
        p[i].ay = (p[i].L - p[i].m * g) / p[i].m;

        if (h > 0 && p[i].ay > 0) {
            p[i].t = sqrt(2.0 * h / p[i].ay);
        } else {
            p[i].t = -1; // cannot climb
        }
    }

    for (int i = 0; i < 3; i++) {
        cout << "Plane " << (i + 1) << ":\n";
        cout << "L = " << p[i].L << "\n";
        cout << "D = " << p[i].D << "\n";
        cout << "a = " << p[i].a << "\n";
        cout << "ay = " << p[i].ay << "\n";

        if (p[i].t < 0) cout << "t = cannot climb\n";
        else cout << "t = " << p[i].t << "\n";
    }

    int best = -1;
    double best_t = 1e100;

    for (int i = 0; i < 3; i++) {
        if (p[i].t >= 0 && p[i].t < best_t) {
            best_t = p[i].t;
            best = i;
        }
    }

    if (best == -1) {
        cout << "No plane can climb to height h\n";
    } else {
        cout << "Fastest plane is: " << (best + 1) << "\n";
    }

    return 0;
}
