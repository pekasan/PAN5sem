#include <iostream>
#include <cmath>
using namespace std;

struct Aircraft {
    double m;
    double T;
    double CL;
    double CD;

    double L;
    double ay;
    double t;

    int id;
};

int main() {
    const double g = 9.81;

    int n;
    double h, V, rho, S;

    cout << "Enter number of aircraft n:\n";
    cin >> n;

    cout << "Enter height h:\n";
    cin >> h;

    cout << "Enter speed V:\n";
    cin >> V;

    cout << "Enter air density rho:\n";
    cin >> rho;

    cout << "Enter wing area S:\n";
    cin >> S;

    Aircraft a[100];

    for (int i = 0; i < n; i++) {
        a[i].id = i + 1;

        cout << "Aircraft " << a[i].id << "\n";

        cout << "Enter mass m:\n";
        cin >> a[i].m;

        cout << "Enter thrust T:\n";
        cin >> a[i].T;

        cout << "Enter CL:\n";
        cin >> a[i].CL;

        cout << "Enter CD:\n";
        cin >> a[i].CD;

        a[i].L = 0.5 * rho * V * V * S * a[i].CL;
        a[i].ay = (a[i].L - a[i].m * g) / a[i].m;

        if (h > 0 && a[i].ay > 0) a[i].t = sqrt(2.0 * h / a[i].ay);
        else a[i].t = -1;
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            bool left_bad = (a[j].t < 0);
            bool right_bad = (a[j + 1].t < 0);

            bool need_swap = false;

            if (left_bad && !right_bad) {
                need_swap = true;
            } else if (!left_bad && !right_bad && a[j].t > a[j + 1].t) {
                need_swap = true;
            }

            if (need_swap) {
                Aircraft tmp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = tmp;
            }
        }
    }

    cout << "Sorted results (by climb time):\n";

    for (int i = 0; i < n; i++) {
        cout << "Aircraft " << a[i].id << ": ";

        if (a[i].t < 0) {
            cout << "cannot climb (ay = " << a[i].ay << ")\n";
        } else {
            cout << "ay = " << a[i].ay << ", t = " << a[i].t << "\n";
        }
    }

    return 0;
}
