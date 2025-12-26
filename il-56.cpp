#include <cmath>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

const double MASS0 = 155000.0;
const double S_WING = 300.0;
const int ENGINE_COUNT = 4;
const double THRUST_PERCENT = 110.0;

const double H_START = 500.0;
const double H_FINISH = 7000.0;
const double V_START_KMH = 330.0;
const double V_FINISH_KMH = 850.0;

const int N = 10;
const double G = 9.81;
const double DEG_TO_RAD = 57.3;

const double MAX_VERTICAL_SPEED = 8.0;
const double MAX_CLIMB_ANGLE = 15.0;
const double MIN_CLIMB_SPEED_KMH = 350.0;

enum OptimizationCriterion {
    MIN_TIME = 1,
    MIN_FUEL = 2
};

enum ManeuverType {
    ACCELERATION = 1,
    CLIMB = 2,
    ACCELERATION_CLIMB = 3
};

struct AtmosPoint {
    double H, rho, a, T;
};

static const AtmosPoint ATMOS_TABLE[] = {
    {0.0,     1.22500, 340.294, 288.150},
    {500.0,   1.16727, 338.370, 284.900},
    {1000.0,  1.11166, 336.435, 281.651},
    {2000.0,  1.00655, 332.532, 275.154},
    {3000.0,  0.90254, 328.584, 268.659},
    {4000.0,  0.81935, 324.589, 262.166},
    {5000.0,  0.73643, 320.545, 255.676},
    {6000.0,  0.66011, 316.452, 249.187},
    {7000.0,  0.59002, 312.306, 242.700},
    {8000.0,  0.52678, 308.105, 236.215},
    {9000.0,  0.46706, 303.848, 229.733},
    {10000.0, 0.41351, 299.532, 223.252},
    {11000.0, 0.36480, 295.154, 216.774}
};
const int ATMOS_N = 13;

double interpolate(double x, double x0, double x1, double y0, double y1) {
    if (abs(x1 - x0) < 1e-9) return y0;
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

void atmosphere(double H, double& rho, double& a_sound) {
    if (H <= ATMOS_TABLE[0].H) {
        rho = ATMOS_TABLE[0].rho;
        a_sound = ATMOS_TABLE[0].a;
        return;
    }
    if (H >= ATMOS_TABLE[ATMOS_N - 1].H) {
        rho = ATMOS_TABLE[ATMOS_N - 1].rho;
        a_sound = ATMOS_TABLE[ATMOS_N - 1].a;
        return;
    }

    for (int i = 0; i < ATMOS_N - 1; i++) {
        if (H >= ATMOS_TABLE[i].H && H < ATMOS_TABLE[i + 1].H) {
            rho = interpolate(H, ATMOS_TABLE[i].H, ATMOS_TABLE[i + 1].H,
                ATMOS_TABLE[i].rho, ATMOS_TABLE[i + 1].rho);
            a_sound = interpolate(H, ATMOS_TABLE[i].H, ATMOS_TABLE[i + 1].H,
                ATMOS_TABLE[i].a, ATMOS_TABLE[i + 1].a);
            return;
        }
    }
}

bool is_in_flight_envelope(double H, double V_kmh) {
    if (V_kmh < 200.0 || V_kmh > 900.0) return false;
    if (H < 0.0 || H > 11000.0) return false;

    double rho, a_sound;
    atmosphere(H, rho, a_sound);
    double M = (V_kmh / 3.6) / a_sound;

    if (M > 0.85) return false;

    return true;
}

const double CY0 = -0.06;
const double CY1 = 0.080;

double Cx_alpha(double alpha_deg) {
    if (alpha_deg < 0.0) alpha_deg = 0.0;
    if (alpha_deg > 12.0) alpha_deg = 12.0;

    double Cx0 = 0.018;
    double k = 0.028;

    double Cy = CY0 + CY1 * alpha_deg;
    double Cx = Cx0 + k * Cy * Cy;

    if (Cx < 0.020) Cx = 0.020;

    return Cx;
}

double thrust_single_pd14_nominal(double H, double M) {
    double H_km = H / 1000.0;
    double P_sea = 58860.0;

    double altitude_factor;
    if (H_km <= 0) {
        altitude_factor = 1.0;
    }
    else if (H_km >= 11.0) {
        altitude_factor = 0.55;
    }
    else {
        altitude_factor = 1.0 - 0.45 * pow(H_km / 11.0, 0.65);
    }

    double mach_factor = 0.92 + 0.22 * M;
    if (mach_factor > 1.10) mach_factor = 1.10;

    return P_sea * altitude_factor * mach_factor;
}

double total_thrust(double H, double V_ms) {
    double rho, a_sound;
    atmosphere(H, rho, a_sound);
    double M = V_ms / a_sound;

    double P_single = thrust_single_pd14_nominal(H, M);
    return P_single * ENGINE_COUNT * (THRUST_PERCENT / 100.0);
}

double specific_fuel_consumption(double H, double V_ms, double power_setting) {
    double rho, a_sound;
    atmosphere(H, rho, a_sound);
    double M = V_ms / a_sound;
    double H_km = H / 1000.0;

    double Cp_base = 0.58;

    double regime_factor;
    if (power_setting >= 1.0) {
        regime_factor = 1.0 + 0.35 * pow(power_setting - 1.0, 1.1);
    }
    else if (power_setting >= 0.90) {
        regime_factor = 0.90 - 0.01 * (power_setting - 0.90) / 0.10;
    }
    else if (power_setting >= 0.75) {
        regime_factor = 0.90 + 0.10 * pow((0.90 - power_setting) / 0.15, 1.0);
    }
    else {
        regime_factor = 1.08;
    }

    double altitude_factor = 1.0 - 0.06 * min(1.0, H_km / 11.0);
    double mach_factor = 1.0 + 0.12 * max(0.0, M - 0.5);

    double Cp = Cp_base * regime_factor * altitude_factor * mach_factor;
    return Cp / 9.81;
}

double calculate_alpha(double H, double V_ms, double mass) {
    double rho, a_sound;
    atmosphere(H, rho, a_sound);

    double P = total_thrust(H, V_ms);
    double q = 0.5 * rho * V_ms * V_ms;

    if (q < 100.0) return 6.0;

    double alpha_deg = (mass * G - P / DEG_TO_RAD - CY0 * q * S_WING) / (CY1 * q * S_WING);

    if (alpha_deg < 0.0) alpha_deg = 0.0;
    if (alpha_deg > 10.0) alpha_deg = 10.0;

    return alpha_deg;
}

struct SegmentData {
    double time;
    double fuel;
    bool valid;
};

SegmentData calculate_acceleration(double H, double V1_ms, double V2_ms, double mass, double power_setting) {
    SegmentData result;
    result.valid = false;
    result.time = 1e9;
    result.fuel = 1e9;

    if (!is_in_flight_envelope(H, V1_ms * 3.6) || !is_in_flight_envelope(H, V2_ms * 3.6)) {
        return result;
    }

    double V_avg = 0.5 * (V1_ms + V2_ms);
    double alpha_deg = calculate_alpha(H, V_avg, mass);
    double alpha_rad = alpha_deg / DEG_TO_RAD;

    double P_max = total_thrust(H, V_avg);
    double P_used = P_max * power_setting;

    double rho, a_sound;
    atmosphere(H, rho, a_sound);
    double q = 0.5 * rho * V_avg * V_avg;

    double Cx = Cx_alpha(alpha_deg);
    double X = Cx * q * S_WING;

    double dV_dt = (P_used * cos(alpha_rad) - X) / mass;

    if (dV_dt <= 0.01) return result;

    double dt = (V2_ms - V1_ms) / dV_dt;

    if (dt > 800.0 || dt <= 0) return result;

    double c_p = specific_fuel_consumption(H, V_avg, power_setting);
    double fuel = c_p * P_used * dt / 3600.0;

    result.time = dt;
    result.fuel = fuel;
    result.valid = true;

    return result;
}

SegmentData calculate_climb(double H1, double H2, double V_ms, double mass, double power_setting, double max_vy_factor) {
    SegmentData result;
    result.valid = false;
    result.time = 1e9;
    result.fuel = 1e9;

    if (V_ms * 3.6 < MIN_CLIMB_SPEED_KMH) return result;
    if (!is_in_flight_envelope(H1, V_ms * 3.6) || !is_in_flight_envelope(H2, V_ms * 3.6)) {
        return result;
    }

    double H_avg = 0.5 * (H1 + H2);
    double alpha_deg = calculate_alpha(H_avg, V_ms, mass);

    double P_max = total_thrust(H_avg, V_ms);
    double P_used = P_max * power_setting;

    double rho, a_sound;
    atmosphere(H_avg, rho, a_sound);
    double q = 0.5 * rho * V_ms * V_ms;

    double Cx = Cx_alpha(alpha_deg);
    double X = Cx * q * S_WING;

    double P_excess = P_used - X;

    if (P_excess <= 0) return result;

    double theta_max_rad = MAX_CLIMB_ANGLE / DEG_TO_RAD;
    double sin_theta = min(P_excess / (mass * G), sin(theta_max_rad));

    if (sin_theta <= 0.005) return result;

    double Vy = V_ms * sin_theta;

    double max_vy_limit = MAX_VERTICAL_SPEED * max_vy_factor;
    if (Vy > max_vy_limit) {
        Vy = max_vy_limit;
    }

    double dt = (H2 - H1) / Vy;

    if (dt <= 0 || dt > 1500.0) return result;

    double c_p = specific_fuel_consumption(H_avg, V_ms, power_setting);
    double fuel = c_p * P_used * dt / 3600.0;

    result.time = dt;
    result.fuel = fuel;
    result.valid = true;

    return result;
}

SegmentData calculate_acceleration_climb(double H1, double H2, double V1_ms, double V2_ms, double mass, double power_setting, double max_vy_factor) {
    SegmentData result;
    result.valid = false;
    result.time = 1e9;
    result.fuel = 1e9;

    double V_avg = 0.5 * (V1_ms + V2_ms);
    double H_avg = 0.5 * (H1 + H2);

    if (V_avg * 3.6 < (MIN_CLIMB_SPEED_KMH * 0.95)) return result;
    if (!is_in_flight_envelope(H1, V1_ms * 3.6) || !is_in_flight_envelope(H2, V2_ms * 3.6)) {
        return result;
    }

    SegmentData acceleration = calculate_acceleration(H_avg, V1_ms, V2_ms, mass, power_setting);
    SegmentData climb = calculate_climb(H1, H2, V_avg, mass, power_setting, max_vy_factor);

    if (!acceleration.valid || !climb.valid) return result;

    double dH = H2 - H1;
    double dV_kmh = (V2_ms - V1_ms) * 3.6;

    double time_for_climb = dH / 6.0;
    double time_for_accel = abs(dV_kmh) / 20.0;

    double dt = max(time_for_climb, time_for_accel);

    if (dt <= 0 || dt > 2000.0) return result;

    double Vy = dH / dt;
    double max_vy_limit = MAX_VERTICAL_SPEED * max_vy_factor * 1.2;

    if (Vy < 0.3 || Vy > max_vy_limit) return result;

    double dV_dt = (V2_ms - V1_ms) / dt;
    if (abs(dV_dt) > 6.0) return result;

    double P_max = total_thrust(H_avg, V_avg);
    double P_used = P_max * power_setting;

    double c_p = specific_fuel_consumption(H_avg, V_avg, power_setting);
    double fuel = c_p * P_used * dt / 3600.0;

    result.time = dt;
    result.fuel = fuel;
    result.valid = true;

    return result;
}

struct TrajectoryResult {
    vector<pair<double, double>> path;
    vector<ManeuverType> maneuvers;
    vector<double> segment_times;
    vector<double> segment_fuels;
    double total_time;
    double total_fuel;
    double avg_vy;
    int used_acceleration;
    int used_climb;
    int used_combined;
    string name;
};

TrajectoryResult solve_trajectory(OptimizationCriterion criterion, string traj_name) {
    TrajectoryResult trajectory;
    trajectory.name = traj_name;

    cout << "\n========================================\n";
    if (criterion == MIN_TIME) {
        cout << "CRITERION: MINIMIZE TIME (" << traj_name << ")\n";
    }
    else {
        cout << "CRITERION: MINIMIZE FUEL (" << traj_name << ")\n";
    }
    cout << "========================================\n\n";

    double dH = (H_FINISH - H_START) / N;
    double dV_kmh = (V_FINISH_KMH - V_START_KMH) / N;

    vector<double> H_grid(N + 1);
    vector<double> V_grid_kmh(N + 1);
    vector<double> V_grid_ms(N + 1);

    for (int i = 0; i <= N; i++) {
        H_grid[i] = H_START + i * dH;
        V_grid_kmh[i] = V_START_KMH + i * dV_kmh;
        V_grid_ms[i] = V_grid_kmh[i] / 3.6;
    }

    vector<double> power_settings;
    double max_vy_factor;

    if (criterion == MIN_TIME) {
        power_settings.push_back(1.10);
        power_settings.push_back(1.05);
        power_settings.push_back(1.00);
        max_vy_factor = 1.0;
    }
    else {
        power_settings.push_back(0.90);
        power_settings.push_back(0.85);
        power_settings.push_back(0.80);
        max_vy_factor = 0.70;
    }

    vector<vector<double> > cost_table(N + 1, vector<double>(N + 1, 1e9));
    vector<vector<double> > time_table(N + 1, vector<double>(N + 1, 0.0));
    vector<vector<double> > fuel_table(N + 1, vector<double>(N + 1, 0.0));
    vector<vector<int> > prev_i(N + 1, vector<int>(N + 1, -1));
    vector<vector<int> > prev_j(N + 1, vector<int>(N + 1, -1));
    vector<vector<int> > maneuver_type(N + 1, vector<int>(N + 1, ACCELERATION));

    cost_table[0][0] = 0.0;

    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            if (cost_table[i][j] >= 1e9) continue;

            double H1 = H_grid[i];
            double V1_ms = V_grid_ms[j];

            for (size_t ps = 0; ps < power_settings.size(); ps++) {
                double power_setting = power_settings[ps];

                if (j < N) {
                    double V2_ms = V_grid_ms[j + 1];
                    SegmentData seg = calculate_acceleration(H1, V1_ms, V2_ms, MASS0, power_setting);

                    if (seg.valid) {
                        double cost_increment = (criterion == MIN_TIME) ? seg.time : seg.fuel;
                        double new_cost = cost_table[i][j] + cost_increment;

                        if (new_cost < cost_table[i][j + 1]) {
                            cost_table[i][j + 1] = new_cost;
                            time_table[i][j + 1] = time_table[i][j] + seg.time;
                            fuel_table[i][j + 1] = fuel_table[i][j] + seg.fuel;
                            prev_i[i][j + 1] = i;
                            prev_j[i][j + 1] = j;
                            maneuver_type[i][j + 1] = ACCELERATION;
                        }
                    }
                }

                if (i < N) {
                    double H2 = H_grid[i + 1];
                    SegmentData seg = calculate_climb(H1, H2, V1_ms, MASS0, power_setting, max_vy_factor);

                    if (seg.valid) {
                        double cost_increment = (criterion == MIN_TIME) ? seg.time : seg.fuel;
                        double new_cost = cost_table[i][j] + cost_increment;

                        if (new_cost < cost_table[i + 1][j]) {
                            cost_table[i + 1][j] = new_cost;
                            time_table[i + 1][j] = time_table[i][j] + seg.time;
                            fuel_table[i + 1][j] = fuel_table[i][j] + seg.fuel;
                            prev_i[i + 1][j] = i;
                            prev_j[i + 1][j] = j;
                            maneuver_type[i + 1][j] = CLIMB;
                        }
                    }
                }

                if (i < N && j < N) {
                    double H2 = H_grid[i + 1];
                    double V2_ms = V_grid_ms[j + 1];
                    SegmentData seg = calculate_acceleration_climb(H1, H2, V1_ms, V2_ms, MASS0, power_setting, max_vy_factor);

                    if (seg.valid) {
                        double cost_increment = (criterion == MIN_TIME) ? seg.time : seg.fuel;
                        double new_cost = cost_table[i][j] + cost_increment;

                        if (new_cost < cost_table[i + 1][j + 1]) {
                            cost_table[i + 1][j + 1] = new_cost;
                            time_table[i + 1][j + 1] = time_table[i][j] + seg.time;
                            fuel_table[i + 1][j + 1] = fuel_table[i][j] + seg.fuel;
                            prev_i[i + 1][j + 1] = i;
                            prev_j[i + 1][j + 1] = j;
                            maneuver_type[i + 1][j + 1] = ACCELERATION_CLIMB;
                        }
                    }
                }
            }
        }
    }

    string suffix = (criterion == MIN_TIME) ? "min_time" : "min_fuel";

    ofstream time_csv("TY-134_time_matrix_" + suffix + ".csv");
    time_csv << "H/V";
    for (int j = 0; j <= N; j++) {
        time_csv << "," << V_grid_kmh[j];
    }
    time_csv << "\n";

    for (int i = 0; i <= N; i++) {
        time_csv << H_grid[i];
        for (int j = 0; j <= N; j++) {
            time_csv << ",";
            if (time_table[i][j] < 1e8) {
                time_csv << time_table[i][j];
            }
        }
        time_csv << "\n";
    }
    time_csv.close();

    ofstream fuel_csv("TY-134_fuel_matrix_" + suffix + ".csv");
    fuel_csv << "H/V";
    for (int j = 0; j <= N; j++) {
        fuel_csv << "," << V_grid_kmh[j];
    }
    fuel_csv << "\n";

    for (int i = 0; i <= N; i++) {
        fuel_csv << H_grid[i];
        for (int j = 0; j <= N; j++) {
            fuel_csv << ",";
            if (fuel_table[i][j] < 1e8) {
                fuel_csv << fuel_table[i][j];
            }
        }
        fuel_csv << "\n";
    }
    fuel_csv.close();

    if (cost_table[N][N] >= 1e9) {
        cout << "ERROR: Path not found!\n";
        return trajectory;
    }

    vector<pair<double, double> > path;
    vector<int> path_maneuvers;
    vector<double> seg_times;
    vector<double> seg_fuels;
    int ci = N, cj = N;

    while (ci >= 0 && cj >= 0) {
        path.push_back(make_pair(H_grid[ci], V_grid_kmh[cj]));
        path_maneuvers.push_back(maneuver_type[ci][cj]);

        int pi = prev_i[ci][cj];
        int pj = prev_j[ci][cj];
        if (pi == -1) break;

        if (!(pi == ci && pj == cj)) {
            seg_times.push_back(time_table[ci][cj] - time_table[pi][pj]);
            seg_fuels.push_back(fuel_table[ci][cj] - fuel_table[pi][pj]);
        }

        ci = pi;
        cj = pj;
    }

    reverse(path.begin(), path.end());
    reverse(path_maneuvers.begin(), path_maneuvers.end());
    reverse(seg_times.begin(), seg_times.end());
    reverse(seg_fuels.begin(), seg_fuels.end());

    ofstream traj_csv("TY-134_trajectory_" + suffix + ".csv");
    traj_csv << "Point,H_m,V_kmh,Maneuver,Segment_time_s,Segment_fuel_kg\n";

    for (size_t k = 0; k < path.size(); k++) {
        traj_csv << k + 1 << ","
            << path[k].first << ","
            << path[k].second << ",";

        if (k == 0) {
            traj_csv << "START,0,0";
        }
        else {
            string maneuver_str;
            if (path_maneuvers[k] == ACCELERATION) maneuver_str = "ACCELERATION";
            else if (path_maneuvers[k] == CLIMB) maneuver_str = "CLIMB";
            else if (path_maneuvers[k] == ACCELERATION_CLIMB) maneuver_str = "ACCELERATION_CLIMB";

            traj_csv << maneuver_str << ","
                << seg_times[k - 1] << ","
                << seg_fuels[k - 1];
        }
        traj_csv << "\n";
    }
    traj_csv.close();

    int used_acceleration = 0, used_climb = 0, used_acceleration_climb = 0;
    for (size_t k = 1; k < path_maneuvers.size(); k++) {
        if (path_maneuvers[k] == ACCELERATION) used_acceleration++;
        else if (path_maneuvers[k] == CLIMB) used_climb++;
        else if (path_maneuvers[k] == ACCELERATION_CLIMB) used_acceleration_climb++;
    }

    cout << "Time matrix (s):\n";
    cout << "     V->";
    for (int j = 0; j <= N; j++) {
        cout << setw(7) << (int)V_grid_kmh[j];
    }
    cout << "\nH\n";
    for (int i = 0; i <= N; i++) {
        cout << setw(5) << (int)H_grid[i];
        for (int j = 0; j <= N; j++) {
            if (time_table[i][j] < 1e8) {
                cout << setw(7) << (int)time_table[i][j];
            }
            else {
                cout << setw(7) << "---";
            }
        }
        cout << "\n";
    }

    cout << "\nFuel consumption matrix (kg):\n";
    cout << "     V->";
    for (int j = 0; j <= N; j++) {
        cout << setw(7) << (int)V_grid_kmh[j];
    }
    cout << "\nH\n";
    for (int i = 0; i <= N; i++) {
        cout << setw(5) << (int)H_grid[i];
        for (int j = 0; j <= N; j++) {
            if (fuel_table[i][j] < 1e8) {
                cout << setw(7) << (int)fuel_table[i][j];
            }
            else {
                cout << setw(7) << "---";
            }
        }
        cout << "\n";
    }
    cout << "\n";

    cout << "Optimal trajectory:\n";
    cout << "-------------------------------------------------------------\n";
    cout << "Point\tH (m)\t\tV (km/h)\tManeuver\tTime\tFuel\n";
    cout << "-------------------------------------------------------------\n";

    for (size_t k = 0; k < path.size(); k++) {
        cout << k + 1 << "\t" << fixed << setprecision(1)
            << setw(8) << path[k].first
            << "\t" << setw(8) << path[k].second << "\t";

        if (k > 0) {
            if (path_maneuvers[k] == ACCELERATION) cout << "Acceleration\t";
            else if (path_maneuvers[k] == CLIMB) cout << "Climb\t\t";
            else if (path_maneuvers[k] == ACCELERATION_CLIMB) cout << "Acc+Climb\t";

            cout << setw(6) << seg_times[k - 1] << "\t"
                << setw(6) << seg_fuels[k - 1];
        }
        else {
            cout << "Start\t\t0\t0";
        }
        cout << "\n";
    }

    cout << "\n=============================================\n";
    cout << "Used in trajectory:\n";
    cout << "- Acceleration: " << used_acceleration << " times\n";
    cout << "- Climb: " << used_climb << " times\n";
    cout << "- Acceleration+Climb: " << used_acceleration_climb << " times\n";
    cout << "---------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Maneuver time:     " << time_table[N][N] << " s  ("
        << time_table[N][N] / 60.0 << " min)\n";
    cout << "Fuel consumption:  " << fuel_table[N][N] << " kg\n";

    double delta_H = H_FINISH - H_START;
    double avg_climb_rate = delta_H / time_table[N][N];

    cout << "Average Vy:        " << avg_climb_rate << " m/s  ("
        << avg_climb_rate * 60.0 << " m/min)\n";

    cout << "\nCreated files:\n";
    cout << "- TY-134_trajectory_" << suffix << ".csv\n";
    cout << "- TY-134_time_matrix_" << suffix << ".csv\n";
    cout << "- TY-134_fuel_matrix_" << suffix << ".csv\n";
    cout << "=============================================\n";

    trajectory.path = path;
    vector<ManeuverType> maneuvers_int;
    for (int m : path_maneuvers) {
        maneuvers_int.push_back(static_cast<ManeuverType>(m));
    }
    trajectory.maneuvers = maneuvers_int;
    trajectory.segment_times = seg_times;
    trajectory.segment_fuels = seg_fuels;
    trajectory.total_time = time_table[N][N];
    trajectory.total_fuel = fuel_table[N][N];
    trajectory.avg_vy = avg_climb_rate;
    trajectory.used_acceleration = used_acceleration;
    trajectory.used_climb = used_climb;
    trajectory.used_combined = used_acceleration_climb;

    return trajectory;
}

int main() {
    cout << fixed << setprecision(2);

    cout << "\n=================================================\n";
    cout << "   TY-134 TRAJECTORY OPTIMIZATION (Variant 15)\n";
    cout << "=================================================\n";
    cout << "Aircraft: TY-134 (mass " << MASS0 / 1000.0 << " t)\n";
    cout << "Engines: 2 x PD-14 (" << THRUST_PERCENT << "% of nominal)\n";
    cout << "Start: H = " << H_START << " m, V = " << V_START_KMH << " km/h\n";
    cout << "Finish: H = " << H_FINISH << " m, V = " << V_FINISH_KMH << " km/h\n";
    cout << "=================================================\n\n";

    int choice;
    cout << "Select optimization criterion:\n";
    cout << "1 - Minimize time\n";
    cout << "2 - Minimize fuel consumption\n";
    cout << "3 - Compare both criteria\n";
    cout << "Your choice (1, 2 or 3): ";
    cin >> choice;

    if (choice == 1) {
        TrajectoryResult result = solve_trajectory(MIN_TIME, "min_time");
    }
    else if (choice == 2) {
        TrajectoryResult result = solve_trajectory(MIN_FUEL, "min_fuel");
    }
    else if (choice == 3) {
        TrajectoryResult traj_time = solve_trajectory(MIN_TIME, "min_time");
        TrajectoryResult traj_fuel = solve_trajectory(MIN_FUEL, "min_fuel");
    }
    else {
        cout << "\nInvalid choice!\n";
    }

    cout << "\nProgram completed.\n";
    return 0;
}
