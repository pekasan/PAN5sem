#include <iostream>
using namespace std;

double drag(double S, double V, double rho, double CD) {
    return 0.5 * rho * V * V * S * CD;
}

int main() {
    double S, V, rho, CD;

    cout << "Enter S:\n";
    cin >> S;

    cout << "Enter V:\n";
    cin >> V;

    cout << "Enter rho:\n";
    cin >> rho;

    cout << "Enter CD:\n";
    cin >> CD;

    double D = drag(S, V, rho, CD);

    cout << "D = " << D << "\n";
    return 0;
}
