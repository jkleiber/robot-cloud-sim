
#pragma once

#include <memory>

#include "core/logging.h"

#include "boat/boat_data.h"
#include "boat/boat_dynamics.h"
#include "boat/boat_server.h"

class BoatSimulator
{
public:
    BoatSimulator() {}
    ~BoatSimulator() {}

    bool Init();
    bool Run();

private:
    // Server and dynamics for the boat.
    std::shared_ptr<BoatServer> server_;
    std::shared_ptr<BoatDynamics> boat_sys_;

    // Simulator data
    BoatData boat_data_;

    // Time
    double t_;
};
