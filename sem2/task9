#include <iostream>
using namespace std;

struct Aircraft {
    double m, S, T, CL, CD;
    double L, D, a;
    int id;
};

int main() {
    int N;
    double V, rho;

    cout << "Enter N:\n";
    cin >> N;

    cout << "Enter V:\n";
    cin >> V;

    cout << "Enter rho:\n";
    cin >> rho;

    Aircraft a[100];

    for (int i = 0; i < N; i++) {
        a[i].id = i + 1;

        cout << "Aircraft " << a[i].id << "\n";

        cout << "Enter m:\n";
        cin >> a[i].m;

        cout << "Enter S:\n";
        cin >> a[i].S;

        cout << "Enter T:\n";
        cin >> a[i].T;

        cout << "Enter CL:\n";
        cin >> a[i].CL;

        cout << "Enter CD:\n";
        cin >> a[i].CD;

        a[i].L = 0.5 * rho * V * V * a[i].S * a[i].CL;
        a[i].D = 0.5 * rho * V * V * a[i].S * a[i].CD;
        a[i].a = (a[i].T - a[i].D) / a[i].m;
    }

    int best = 0;
    for (int i = 1; i < N; i++) {
        if (a[i].a > a[best].a) best = i;
    }

    for (int i = 0; i < N; i++) {
        cout << "Aircraft " << a[i].id << ":\n";
        cout << "L = " << a[i].L << "\n";
        cout << "D = " << a[i].D << "\n";
        cout << "acceleration a = " << a[i].a << "\n";
    }

    cout << "Max acceleration aircraft: " << a[best].id << "\n";

    return 0;
}
