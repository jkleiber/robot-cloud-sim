
#pragma once

#include <eigen3/Eigen/Dense>

#include "boat/boat_data.h"
#include "core/logging.h"
#include "core/math_utils.h"

// TODO: parameterize boat limitations
const double kBoatMaxSpeed = 10; // m/s, approx. 20 kts.
const double kPropMaxRPM = 7600; // rpm, approx. 800 rad/s.
const double kRPMPerRad = 9.5492968;

class BoatDynamics
{
public:
    BoatDynamics(BoatState *state, BoatControl *ctrl, const double &dt)
        : state_(state), ctrl_(ctrl)
    {
    }
    ~BoatDynamics() {}

    bool Init();

    bool Update();

private:
    BoatState *const state_;
    BoatControl *const ctrl_;
    double dt_;

    // Subsystem updates.
    bool UpdatePropulsion(Eigen::VectorXd *xdot);
    bool UpdateVelocity(Eigen::VectorXd *xdot);
    bool UpdateYaw(Eigen::VectorXd *xdot);

    // Subsystem states.
    // Propellor
    Eigen::VectorXd prop_x_;
    // Boat Velocity
    Eigen::VectorXd vel_x_;
    // Yaw
    Eigen::VectorXd yaw_x_;

    // Subsystem dynamics
    // Propellor
    Eigen::MatrixXd prop_A_;
    Eigen::VectorXd prop_B_;
    // Velocity
    Eigen::MatrixXd vel_A_;
    Eigen::VectorXd vel_B_;
    // Yaw
    Eigen::MatrixXd yaw_A_;
    Eigen::VectorXd yaw_B_;
};
