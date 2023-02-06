
#pragma once

#include <eigen3/Eigen/Dense>

#include "boat/boat_data.h"

class BoatDynamics
{
public:
    BoatDynamics(BoatState *state, const double &dt) : state_(state) {}
    ~BoatDynamics() {}

    bool Init();

    bool Update(Eigen::VectorXd u);

private:
    BoatState *const state_;
    double dt_;

    Eigen::VectorXd x_;
};