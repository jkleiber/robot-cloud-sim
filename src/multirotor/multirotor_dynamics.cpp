#include "multirotor/multirotor_dynamics.h"

void MultirotorDynamics::Init(Eigen::VectorXd x0) {}

void MultirotorDynamics::UpdateDynamics(Eigen::VectorXd u0)
{
    // States
    double u = x(0);
    double v = x(1);
    double w = x(2);
    double p = x(3);
    double q = x(4);
    double r = x(5);
    double x_nav = x(6);
    double y_nav = x(7);
    double z_nav = x(8);
    double roll = x(9);
    double pitch = x(10);
    double yaw = x(11);

    // Compute motor forces from control.
    this->GetMotorForces(u0);

    // Motor Numbering
    /*
        3 --[---]---1
            |   |
        2---[---]---4
    */
    // Roll to the right is positive
    // Pitch up is positive
    // CW yaw is positive

    // Control is the motor forces
    double F1 = x_motors(0);
    double F2 = x_motors(1);
    double F3 = x_motors(2);
    double F4 = x_motors(3);

    // "Virtual" control
    double Fz = F1 + F2 + F3 + F4;
    double L = (F1 - F2 - F3 + F4) * dy_arm;
    double M = (F1 - F2 + F3 - F4) * dx_arm;

    // w^2 = F / K_f
    // torque = K_m w^2 = K_b (F / K_f)
    // Note: this assumes CW is positive
    double N = K_m * (-F1 - F2 + F3 + F4) / K_f;

    // Angles
    double cos_roll = cos(roll);
    double cos_pitch = cos(pitch);
    double cos_yaw = cos(yaw);
    double sin_roll = sin(roll);
    double sin_pitch = sin(pitch);
    double sin_yaw = sin(yaw);

    // Derivative state
    Eigen::VectorXd x_dot = Eigen::VectorXd::Zero(12);

    // Velocity update
    x_dot(0) = -g * sin_pitch + r * v - q * w;
    x_dot(1) = g * sin_roll * cos_pitch - r * u + p * w;
    x_dot(2) = (1 / m) * Fz - g * cos_roll * cos_pitch - q * u + p * v;

    // Angular velocity update
    x_dot(3) = (1 / Ixx) * (L + (Iyy - Izz) * q * r);
    x_dot(4) = (1 / Iyy) * (M + (Izz - Ixx) * p * r);
    x_dot(5) = (1 / Izz) * (N + (Ixx - Iyy) * p * q);

    // Position update
    x_dot(6) = cos_pitch * cos_yaw * u +
               (-cos_roll * sin_yaw + sin_roll * sin_pitch * cos_yaw) * v +
               (sin_roll * sin_yaw + cos_roll * sin_pitch * cos_yaw) * w;
    x_dot(7) = cos_pitch * sin_yaw * u +
               (cos_roll * cos_yaw + sin_roll * sin_pitch * sin_yaw) * v +
               (-sin_roll * cos_yaw + cos_roll * sin_pitch * sin_yaw) * w;
    x_dot(8) =
        -sin_pitch * u + sin_roll * cos_pitch * v + cos_roll * cos_pitch * w;

    // Angle update
    x_dot(9) = p + (q * sin_roll + r * cos_roll) * tan(pitch);
    x_dot(10) = q * cos_pitch - r * sin_roll;
    x_dot(11) = (q * sin_roll + r * cos_roll) * (1 / cos_pitch);

    // Apply the update to the dynamics
    this->x = this->x + x_dot * dt_;

    // Log the dynamics
    state_log.LogVectorXd(this->x);

    // Don't allow the drone to fall below the ground
    // if (x(8) < 0)
    // {
    //     x(8) = 0;
    // }
}

void MultirotorDynamics::LoadDefaultParams()
{
    // Set the mass
    m = 1.00; // kg

    // Set gravity
    g = 9.81;

    // Set the inertia
    // TODO: Make this configurable
    Ixx = 0.007; // kg/m^2
    Iyy = 0.006; // kg/m^2
    Izz = 0.009; // kg/m^2

    // Limit the motor's slew rate
    motor_slew_rate = 0.4 / 0.01; // Limit to changes of 40% every 0.01 sec

    // Force and moment constants. F = K_f w^2, M = K_b w^2
    // TODO: This is arbitrary, but sort of based on the motor specs
    // Max force is 1.522 kg for a 5.1x3.1x3 propeller. This assumes 16V when we
    // have 14.8V equipped. Let's take 1.522 * 14.8 / 16 = 1.40785 kg to be max
    // pull. (Force = 1.40785 * 9.81 = 13.811).
    max_motor_force = 13.811;

    // Max RPM (no-load) is 2750 KV * 14.8V = 40,700 RPM -> 4262 rad/s.
    // Assuming a load makes for 80% max speed -> 3409.6 rad/s
    max_omega = 3409.6;

    // Based on nothing but the feeling that torque of the props is much smaller
    // than the force of the props, we divide K_f by 20 to get K_m
    Kf_divisor_ = 20.0;

    // Simplify Ixy, Ixz, Iyz = 0 since they are all very small
    Ixy = 0;
    Ixz = 0;
    Iyz = 0;

    // Arm length
    dx_arm = 0.097;
    dy_arm = 0.15; // m

    // Disturbances
    dist_mean = 0.0;
    dist_stddev = 0.5;
}
