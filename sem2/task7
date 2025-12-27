#include <iostream>
using namespace std;

int main() {
    double m, T, D;

    cout << "Enter mass m:\n";
    cin >> m;

    cout << "Enter thrust T:\n";
    cin >> T;

    cout << "Enter drag D:\n";
    cin >> D;

    double a = (T - D) / m;

    cout << "a = " << a << "\n";

    if (a > 0.5) {
        cout << "Mode: climb\n";
    } else if (a >= 0) {
        cout << "Mode: level flight\n";
    } else {
        cout << "Mode: descent\n";
    }

    return 0;
}
