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

    if (heights.empty()) {
        cout << "No data\n";
        return 0;
    }

    size_t idx_max = 0;
    for (size_t i = 1; i < heights.size(); i++) {
        if (heights[i] > heights[idx_max]) idx_max = i;
    }

    cout << fixed << setprecision(2);
    cout << "Max height = " << heights[idx_max] << "\n";
    cout << "Time at max = " << times[idx_max] << "\n";

    return 0;
}
