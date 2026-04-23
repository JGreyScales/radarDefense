#include <iostream>
#include <cmath>
#include <vector>

// Physics Constants
const double G = 9.81;           // Gravity (m/s^2)
const double RHO = 1.225;        // Air density (kg/m^3)
const double A = 0.05;           // Reference area (m^2)
const double CD = 0.3;           // Drag Coefficient
const double CL = 0.1;           // Lift Coefficient
const double DT = 0.01;          // Time step (seconds)

struct State {
    double x = 0.0, y = 0.0;
    double vx = 0.0, vy = 0.0;
    double mass = 10.0;          // Initial mass (kg)
    double time = 0.0;
};

int main() {
    // Projectile Parameters
    double thrust = 500.0;       // Newtons
    double burn_time = 5.0;      // Seconds
    double fuel_burn_rate = 0.5; // kg/s
    double pitch_angle = 45.0 * (M_PI / 180.0); // Launch angle in radians

    State s;
    
    std::cout << "Time(s), X(m), Y(m), Velocity(m/s)" << std::endl;

    // Simulation Loop
    while (s.y >= 0) {
        // 1. Determine active forces
        double current_thrust = (s.time <= burn_time) ? thrust : 0.0;
        
        // Update mass if still burning
        if (s.time <= burn_time) {
            s.mass -= fuel_burn_rate * DT;
        }

        // 2. Calculate Aerodynamics
        double speed = std::sqrt(s.vx * s.vx + s.vy * s.vy);
        double gamma = std::atan2(s.vy, s.vx); // Flight path angle
        
        double drag = 0.5 * RHO * speed * speed * A * CD;
        double lift = 0.5 * RHO * speed * speed * A * CL;

        // 3. Resolve Forces into Global Axes
        // Thrust vector acts at pitch_angle
        // Drag acts against velocity vector (gamma)
        // Lift acts perpendicular to velocity vector (gamma + 90)
        double Fx = current_thrust * std::cos(pitch_angle) 
                  - drag * std::cos(gamma) 
                  - lift * std::sin(gamma);

        double Fy = current_thrust * std::sin(pitch_angle) 
                  - drag * std::sin(gamma) 
                  + lift * std::cos(gamma) 
                  - (s.mass * G);

        // 4. Update Velocity and Position (Euler Integration)
        double ax = Fx / s.mass;
        double ay = Fy / s.mass;

        s.vx += ax * DT;
        s.vy += ay * DT;
        s.x += s.vx * DT;
        s.y += s.vy * DT;
        s.time += DT;

        // Print status every second
        if (fmod(s.time, 1.0) < DT) {
            std::cout << s.time << ", " << s.x << ", " << s.y << ", " << speed << std::endl;
        }
    }

    std::cout << "Impact at: " << s.x << " meters." << std::endl;
    return 0;
}