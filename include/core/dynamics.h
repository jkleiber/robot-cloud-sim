
#pragma once

#include <eigen3/Eigen/Dense>
#include <string>

#include "core/sim_component.h"

class RigidBodyDynamicsEngine : public SimComponent
{
public:
    RigidBodyDynamicsEngine(std::string name) : SimComponent(name) {}
    virtual ~RigidBodyDynamicsEngine() {}

    bool Compute(Eigen::VectorXd *dx);

private:
    /**
     * Inertia matrix
     */
    Eigen::MatrixXd I_;

    /**
     * Mass
     */
    double mass_;

    /**
     * State vector
     */
    Eigen::VectorXd x_;
};
