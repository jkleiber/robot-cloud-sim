
#include "auv/auv_dynamics.h"

bool AUVDynamics::Init()
{
    VERIFY(state_ != nullptr);

    // Initialize the states
    prop_x_ = Eigen::VectorXd::Zero(2);
    vel_x_ = Eigen::VectorXd::Zero(2);
    rud_x_ = Eigen::VectorXd::Zero(2);
    yaw_x_ = Eigen::VectorXd::Zero(2);

    //////////////////////////////////////////////////
    // The below systems were made with
    // tools/linear_system_analysis/Actuators.ipynb
    //////////////////////////////////////////////////

    // Linear system for Propeller RPM
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): motor power (%, 0-1)
    // States (x): Propeller speed (rad/s), Propeller acceleration(rad/s^2)
    prop_A_ = Eigen::MatrixXd::Zero(2, 2);
    prop_A_(0, 0) = -1; // Higher speed -> more drag
    prop_A_(0, 1) = 1.0;
    prop_A_(1, 1) = -2;
    prop_B_ = Eigen::Vector2d::Zero(2);
    prop_B_(1) = 500.0;

    // Linear system for AUV velocity
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): Propeller speed (rad/s)
    // States (x): velocity (m/s), acceleration(m/s^2)
    vel_A_ = Eigen::MatrixXd::Zero(2, 2);
    vel_A_(0, 0) = -0.5; // Higher speed -> more drag
    vel_A_(0, 1) = 1.0;
    vel_A_(1, 0) = -0.0;
    vel_A_(1, 1) = -0.75;
    vel_B_ = Eigen::Vector2d::Zero(2);
    vel_B_(1) = 0.015;

    // Linear system for rudder angle
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): rudder setpoint (rad, +/- 20 deg)
    // States (x): rudder angle (rad), rudder rate (rad/s)
    rud_A_ = Eigen::MatrixXd::Zero(2, 2);
    rud_A_(0, 1) = 1.0;
    rud_A_(1, 0) = -1.0;
    rud_A_(1, 1) = -3;
    rud_B_ = Eigen::Vector2d::Zero(2);
    rud_B_(1) = 1.0;

    // Linear parameter varying system for yaw
    // Equation: xdot = A x(t) + B(p) u(t)
    // Inputs (u): rudder angle (rad)
    // States (x): yaw (rad), yaw rate (rad/s)
    // Parameters (p): velocity (m/s)
    // Note: the B matrix gets interpolated based on the speed.
    yaw_A_ = Eigen::MatrixXd::Zero(2, 2);
    yaw_A_(0, 1) = 1.0;
    yaw_A_(1, 1) = -0.99;
    yaw_B_ = Eigen::Vector2d::Zero(2);
    yaw_B_(1) = 0.05;

    // Set initial conditions
    double yaw_rad = state_->yaw;
    VERIFY(DegToRad(&yaw_rad));
    yaw_x_(0) = yaw_rad;

    return true;
}

bool AUVDynamics::Update()
{
    VERIFY(state_ != nullptr);

    ///// Do subsystem dynamics updates
    // Propulsion Subsystem
    VERIFY(this->UpdatePropulsion(&prop_xdot_));
    VERIFY(this->UpdateVelocity(&vel_xdot_));

    // Lateral Subsystem
    VERIFY(this->UpdateRudder(&rud_xdot_));
    VERIFY(this->UpdateYaw(&yaw_xdot_));

    // Convert dt_ to seconds as a double.
    double dt = static_cast<double>(dt_) / static_cast<double>(kSec);

    // Integrate
    prop_x_ += prop_xdot_ * dt;
    vel_x_ += vel_xdot_ * dt;
    rud_x_ += rud_xdot_ * dt;
    yaw_x_ += yaw_xdot_ * dt;

    // Update the state pointer
    state_->prop_rpm = prop_x_(0) * kRPMPerRad;
    state_->speed = vel_x_(0);

    double yaw_deg = yaw_x_(0);
    VERIFY(RadToDeg(&yaw_deg));
    state_->yaw = yaw_deg;

    // Update the lat/lon.
    VERIFY(this->IntegrateGps(dt));

    return true;
}

bool AUVDynamics::UpdatePropulsion(Eigen::Vector2d *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(ctrl_ != nullptr);

    // Clamp the control input
    VERIFY(Clamp(&ctrl_->power, 0.0, 1.0));

    // Do the linear state update.
    *xdot = prop_A_ * prop_x_ + prop_B_ * ctrl_->power;

    return true;
}

bool AUVDynamics::UpdateVelocity(Eigen::Vector2d *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(state_ != nullptr);

    // Get the prop speed as input in rad/s
    *xdot = vel_A_ * vel_x_ + vel_B_ * prop_x_(0);

    return true;
}

bool AUVDynamics::UpdateRudder(Eigen::Vector2d *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(ctrl_ != nullptr);

    // Clamp the control input to the rudder range of motion.
    VERIFY(Clamp(&ctrl_->rudder, -20.0, 20.0));

    // Convert to radians
    double rudder_setpoint_rad = ctrl_->rudder;
    VERIFY(DegToRad(&rudder_setpoint_rad));

    *xdot = rud_A_ * rud_x_ + rud_B_ * rudder_setpoint_rad;

    return true;
}

bool AUVDynamics::UpdateYaw(Eigen::Vector2d *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(state_ != nullptr);

    // Compute the speed factor.
    // The control is effective to the speed squared, normalized on a range of
    // [0, max speed] -> [0, 1]
    double speed_normalized = state_->speed / kAUVMaxSpeed;
    double speed_factor = speed_normalized * speed_normalized;

    // Do the linear state update
    *xdot = yaw_A_ * yaw_x_ + yaw_B_ * rud_x_(0) * speed_factor;

    return true;
}

bool AUVDynamics::IntegrateGps(double dt)
{
    VERIFY(state_ != nullptr);

    // Find distance travelled (in m) during timestep.
    double dist = state_->speed * dt;

    // Get heading (note that state_->yaw is in degrees so use yaw_x_ in radians
    // instead).
    double hdg = yaw_x_(0);

    // Get current (about to be previous) coordinates.
    double prev_lat = state_->lat;
    double prev_lon = state_->lon;

    // Convert to radians.
    VERIFY(DegToRad(&prev_lat));
    VERIFY(DegToRad(&prev_lon));

    // Compute the next lat/lon (in radians).
    double next_lat = asin(sin(prev_lat) * cos(dist / kEarthRadius) +
                           cos(prev_lat) * sin(dist / kEarthRadius) * cos(hdg));
    double next_lon =
        prev_lon +
        atan2(sin(hdg) * sin(dist / kEarthRadius) * cos(prev_lat),
              cos(dist / kEarthRadius) - sin(prev_lat) * sin(next_lat));

    // Convert back to degrees.
    VERIFY(RadToDeg(&next_lat));
    VERIFY(RadToDeg(&next_lon));

    // Wrap the longitude to (-180.0, 180.0)
    next_lon = fmod((next_lon + 540.0), 360.0) - 180.0;

    // Apply the GPS update.
    state_->lat = next_lat;
    state_->lon = next_lon;

    return true;
}
