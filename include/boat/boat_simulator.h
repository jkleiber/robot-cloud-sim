
#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "core/logging.h"
#include "core/time.h"

#include "boat/boat_data.h"
#include "boat/boat_dynamics.h"
#include "boat/boat_server.h"

const unsigned long long kPrintPeriod = kSec;

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
    unsigned long long t_;
    // Note: this is very important for simulator speed.
    unsigned long long prev_print_t_ = 0;
    // Track speedup from simulator
    std::chrono::time_point<std::chrono::system_clock> prev_print_clock_time_ =
        std::chrono::system_clock::now();
};
