
#pragma once

#include <eigen3/Eigen/Dense>

#include "boat/boat_data.h"
#include "core/logging.h"

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

    Eigen::VectorXd x_;
};