
#pragma once

#include <eigen3/Eigen/Dense>

#include "boat/boat_data.h"
#include "core/logging.h"
#include "core/math_utils.h"
#include "core/time.h"

// TODO: parameterize boat limitations
const double kBoatMaxSpeed = 10; // m/s, approx. 20 kts.
const double kPropMaxRPM = 7600; // rpm, approx. 800 rad/s.
const double kRPMPerRad = 9.5492968;
const double kEarthRadius = 6371000.0; // meters, earth's radius.

class BoatDynamics
{
public:
    BoatDynamics(BoatState *state, BoatControl *ctrl,
                 const unsigned long long &dt)
        : state_(state), ctrl_(ctrl), dt_(dt)
    {
    }
    ~BoatDynamics() {}

    bool Init();

    bool Update();

private:
    BoatState *const state_;
    BoatControl *const ctrl_;
    const unsigned long long &dt_;

    // Subsystem updates.
    bool UpdatePropulsion(Eigen::Vector2d *xdot);
    bool UpdateVelocity(Eigen::Vector2d *xdot);
    bool UpdateRudder(Eigen::Vector2d *xdot);
    bool UpdateYaw(Eigen::Vector2d *xdot);

    // Global updates.
    bool IntegrateGps(double dt);

    // Subsystem states.
    // Propeller
    Eigen::Vector2d prop_x_;
    // Boat Velocity
    Eigen::Vector2d vel_x_;
    // Rudder
    Eigen::Vector2d rud_x_;
    // Yaw
    Eigen::Vector2d yaw_x_;

    // Subsystem derivatives.
    Eigen::Vector2d prop_xdot_;
    Eigen::Vector2d vel_xdot_;
    Eigen::Vector2d rud_xdot_;
    Eigen::Vector2d yaw_xdot_;

    // Subsystem dynamics
    // Propeller
    Eigen::MatrixXd prop_A_;
    Eigen::Vector2d prop_B_;
    // Velocity
    Eigen::MatrixXd vel_A_;
    Eigen::Vector2d vel_B_;
    // Rudder
    Eigen::MatrixXd rud_A_;
    Eigen::Vector2d rud_B_;
    // Yaw
    Eigen::MatrixXd yaw_A_;
    Eigen::Vector2d yaw_B_;
};
