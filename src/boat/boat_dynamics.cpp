
#include "boat/boat_dynamics.h"

bool BoatDynamics::Init()
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
    prop_B_ = Eigen::VectorXd::Zero(2);
    prop_B_(1) = 500.0;

    // Linear system for boat velocity
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): Propeller speed (rad/s)
    // States (x): velocity (m/s), acceleration(m/s^2)
    vel_A_ = Eigen::MatrixXd::Zero(2, 2);
    vel_A_(0, 0) = -0.5; // Higher speed -> more drag
    vel_A_(0, 1) = 1.0;
    vel_A_(1, 0) = -0.0;
    vel_A_(1, 1) = -0.75;
    vel_B_ = Eigen::VectorXd::Zero(2);
    vel_B_(1) = 0.015;

    // Linear system for rudder angle
    // Equation: xdot = A x(t) + B u(t)
    // Inputs (u): rudder setpoint (rad, +/- 20 deg)
    // States (x): rudder angle (rad), rudder rate (rad/s)
    rud_A_ = Eigen::MatrixXd::Zero(2, 2);
    rud_A_(0, 1) = 1.0;
    rud_A_(1, 0) = -1.0;
    rud_A_(1, 1) = -3;
    rud_B_ = Eigen::VectorXd::Zero(2);
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
    yaw_B_ = Eigen::VectorXd::Zero(2);
    yaw_B_(1) = 0.5;

    return true;
}

bool BoatDynamics::Update()
{
    VERIFY(state_ != nullptr);

    // Derivatives
    Eigen::VectorXd prop_xdot = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd vel_xdot = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd rud_xdot = Eigen::VectorXd::Zero(2);
    Eigen::VectorXd yaw_xdot = Eigen::VectorXd::Zero(2);

    ///// Do subsystem dynamics updates
    // Propulsion Subsystem
    VERIFY(this->UpdatePropulsion(&prop_xdot));
    VERIFY(this->UpdateVelocity(&vel_xdot));

    // Lateral Subsystem
    VERIFY(this->UpdateRudder(&rud_xdot));
    VERIFY(this->UpdateYaw(&yaw_xdot));

    // Integrate
    prop_x_ += prop_xdot * dt_;
    vel_x_ += vel_xdot * dt_;
    rud_x_ += rud_xdot * dt_;
    yaw_x_ += yaw_xdot * dt_;

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

    // Get the prop speed as input in rad/s
    double prop_speed = prop_x_(0);

    *xdot = vel_A_ * vel_x_ + vel_B_ * prop_speed;

    return true;
}

bool BoatDynamics::UpdateRudder(Eigen::VectorXd *xdot)
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

bool BoatDynamics::UpdateYaw(Eigen::VectorXd *xdot)
{
    VERIFY(xdot != nullptr);
    VERIFY(state_ != nullptr);

    // Get the speed
    double speed = state_->speed;

    // Compute the speed factor.
    // The control is effective to the speed squared, normalized on a range of
    // [0, max speed] -> [0, 1]
    double speed_normalized = speed / kBoatMaxSpeed;
    double speed_factor = speed_normalized * speed_normalized;

    // Do the linear state update
    *xdot = yaw_A_ * yaw_x_ + yaw_B_ * rud_x_(0) * speed_factor;

    return true;
}
