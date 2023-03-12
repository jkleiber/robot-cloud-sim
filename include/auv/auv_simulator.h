
#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "core/logging.h"
#include "core/time.h"

#include "auv/auv_data.h"
#include "auv/auv_dynamics.h"
#include "auv/auv_msg.pb.h"
#include "auv/auv_server.h"

const unsigned long long kPrintPeriod = kSec;

class AUVSimulator
{
public:
    AUVSimulator() {}
    ~AUVSimulator() {}

    bool Init();
    bool Run();

private:
    // Server and dynamics for the auv.
    std::shared_ptr<AUVServer> server_;
    std::shared_ptr<AUVDynamics> auv_sys_;

    // Simulator data
    AUVData auv_data_;

    // Time
    unsigned long long t_;
    // Note: this is very important for simulator speed.
    unsigned long long prev_print_t_ = 0;
    // Track speedup from simulator
    std::chrono::time_point<std::chrono::system_clock> prev_print_clock_time_ =
        std::chrono::system_clock::now();
};
