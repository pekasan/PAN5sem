#include <iostream>
using namespace std;

int main() {
    int n;
    double S, CL;

    cout << "Enter number of steps n:\n";
    cin >> n;

    cout << "Enter wing area S:\n";
    cin >> S;

    cout << "Enter lift coefficient CL:\n";
    cin >> CL;

    double V[1000];
    double rho[1000];

    cout << "Enter speeds V (n numbers):\n";
    for (int i = 0; i < n; i++) cin >> V[i];

    cout << "Enter densities rho (n numbers):\n";
    for (int i = 0; i < n; i++) cin >> rho[i];

    cout << "Step | Speed | Density | Lift\n";

    for (int i = 0; i < n; i++) {
        double L = 0.5 * rho[i] * V[i] * V[i] * S * CL;
        cout << (i + 1) << " | " << V[i] << " | " << rho[i] << " | " << L << "\n";
    }

    return 0;
}
