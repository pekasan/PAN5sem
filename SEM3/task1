#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

int main() {
    const double v0 = 50.0;
    const double g  = 9.8;
    const double dt = 1.0;

    if (v0 <= 0) {
        cout << "Warning: v0 must be positive\n";
        return 0;
    }

    vector<double> times;
    vector<double> heights;

    int approxSize = (int)(v0 * 2.0 / g / dt) + 5;
    if (approxSize < 0) approxSize = 0;
    times.reserve(approxSize);
    heights.reserve(approxSize);

    for (double t = 0.0; t <= 1000.0; t += dt) {
        double h = v0 * t - 0.5 * g * t * t;

        if (h < 0) break;

        times.push_back(t);
        heights.push_back(h);
    }

    cout << "Time(s)  Height(m)\n";
    cout << fixed << setprecision(2);

    for (size_t i = 0; i < times.size(); i++) {
        cout << setw(7) << times[i] << "  " << setw(9) << heights[i] << "\n";
    }

    return 0;
}
