
#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "core/logging.h"
#include "core/time.h"

#include "boat/boat_data.h"
#include "boat/boat_dynamics.h"
#include "boat/boat_server.h"

#define PRINT_RATE 1.0   // Hz
#define SERVER_RATE 50.0 // Hz

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
    // Note: this is very important for simulator speed.
    double prev_print_t_ = -100;

    // Server update rate tracker
    double prev_serve_t_ = -100;
};
