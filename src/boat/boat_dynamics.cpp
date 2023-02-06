
#include "boat/boat_dynamics.h"

bool BoatDynamics::Init() { return true; }

bool BoatDynamics::Update(Eigen::VectorXd u)
{
    // x_dot = Ax + Bu
    // Eigen::VectorXd x_dot = Eigen::VectorXd::Zero;

    // Integrate in discrete time.
    x_ = x_;

    return true;
}
