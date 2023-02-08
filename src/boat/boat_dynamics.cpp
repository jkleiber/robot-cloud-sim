
#include "boat/boat_dynamics.h"

bool BoatDynamics::Init()
{
    VERIFY(state_ != nullptr);

    // TODO: make this configurable
    state_->lat = 0.0;
    state_->lon = 0.0;
    state_->yaw = 0.0;

    return true;
}

bool BoatDynamics::Update(Eigen::VectorXd u)
{
    // x_dot = Ax + Bu
    // Eigen::VectorXd x_dot = Eigen::VectorXd::Zero;

    // Integrate in discrete time.
    x_ = x_;

    return true;
}
