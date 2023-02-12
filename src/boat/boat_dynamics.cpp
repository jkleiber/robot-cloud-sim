
#include "boat/boat_dynamics.h"

bool BoatDynamics::Init()
{
    VERIFY(state_ != nullptr);

    // Initialize the states
    prop_x_ = Eigen::VectorXd::Zero(2);
    vel_x_ = Eigen::VectorXd::Zero(2);
    yaw_x_ = Eigen::VectorXd::Zero(2);

    // Linear system for propellor RPM
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): motor power (%, 0-1)
    // States (x): propellor speed (rad/s), propellor acceleration(rad/s^2)
    prop_A_ = Eigen::MatrixXd::Zero(2, 2);
    prop_A_(0, 1) = 1.0;
    prop_A_(1, 1) = -0.125;
    prop_B_ = Eigen::VectorXd::Zero(2);
    prop_B_(1) = 100.0;

    // Linear system for boat velocity
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): propellor speed (rad/s)
    // States (x): velocity (m/s), acceleration(m/s^2)
    vel_A_ = Eigen::MatrixXd::Zero(2, 2);
    vel_A_(0, 1) = 1.0;
    vel_A_(1, 1) = -0.8;
    vel_B_ = Eigen::VectorXd::Zero(2);
    vel_B_(1) = 0.01;

    // Linear parameter varying system for yaw
    // Equation: xdot = A x(t) + B(p) u(t)
    // Inputs (u): rudder (rad, converted from deg, +/- 20 deg)
    // States (x): yaw (rad), yaw rate (rad/s)
    // Parameters (p): velocity (m/s)
    // Note: the B matrix gets interpolated based on the speed.
    yaw_A_ = Eigen::MatrixXd::Zero(2, 2);
    yaw_A_(0, 1) = 1.0;
    yaw_A_(1, 1) = -0.95;
    yaw_B_ = Eigen::VectorXd::Zero(2);
    yaw_B_(1) = 0.5;
    // Given the maximum rudder deflection, the choice of values for yaw_A_(1,
    // 1) and yaw_B_(1) determines the maximum yaw rate. Max yaw rate =
    // max_rudder * yaw_B_(1) / abs(yaw_A_(1, 1)).
    //  With these parameters, the maximum yaw rate is 0.184 rad/s, or
    //  about 10.5 deg/s
    // TODO: this might still be too fast.

    return true;
}

bool BoatDynamics::Update()
{
    VERIFY(state_ != nullptr);

    // Derivatives
    Eigen::VectorXd prop_xdot = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd vel_xdot = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd yaw_xdot = Eigen::VectorXd::Zero(2);

    // Do subsystem dynamics updates
    VERIFY(this->UpdatePropulsion(&prop_xdot));
    VERIFY(this->UpdateVelocity(&vel_xdot));
    VERIFY(this->UpdateYaw(&yaw_xdot));

    // Integrate
    prop_x_ += prop_xdot * dt_;
    vel_x_ += vel_xdot * dt_;
    yaw_x_ += yaw_xdot * dt_;

    // Impose limits on the system
    // TODO: make better models that do this on their own.
    VERIFY(Clamp(&prop_x_(0), 0.0, kPropMaxRPM));
    // TODO: decide if the boat should be able to go in reverse. This would
    // require reverse prop as well.
    VERIFY(Clamp(&vel_x_(0), 0.0, kBoatMaxSpeed));

    // Update the state pointer
    state_->prop_rpm = prop_x_(0) * kRPMPerRad;
    state_->speed = vel_x_(0);

    double yaw_rad = yaw_x_(0);
    VERIFY(RadToDeg(&yaw_rad));
    state_->yaw = yaw_rad;

    return true;
}

bool BoatDynamics::UpdatePropulsion(Eigen::VectorXd *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(ctrl_ != nullptr);

    // Clamp the control input
    VERIFY(Clamp(&ctrl_->power, 0.0, 1.0));

    // Do the linear state update.
    *xdot = prop_A_ * prop_x_ + prop_B_ * ctrl_->power;

    return true;
}

bool BoatDynamics::UpdateVelocity(Eigen::VectorXd *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(state_ != nullptr);

    // Get the prop speed as input.

    return true;
}

bool BoatDynamics::UpdateYaw(Eigen::VectorXd *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(state_ != nullptr);
    VERIFY(ctrl_ != nullptr);

    // Clamp the control input
    VERIFY(Clamp(&ctrl_->rudder, -20.0, 20.0));

    // Convert to radians
    double rudder_rad = ctrl_->rudder;
    VERIFY(DegToRad(&rudder_rad));

    // Do the linear state update
    *xdot = yaw_A_ * yaw_x_ + yaw_B_ * rudder_rad;

    return true;
}
