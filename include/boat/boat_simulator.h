
#pragma once

#include <chrono>
#include <memory>
#include <thread>

#include "core/logging.h"
#include "core/simulator.h"
#include "core/time.h"

#include "boat/boat_data.h"
#include "boat/boat_dynamics.h"
#include "boat/boat_server.h"

const unsigned long long kPrintPeriod = kSec;

class BoatSimulator : public Simulator
{
public:
    BoatSimulator() : rpc_thread_() {}
    ~BoatSimulator() { bool result_ = this->Stop(); }

    bool Init();
    bool Start();
    bool Run();
    bool Stop();

private:
    // Simulator data
    BoatData boat_data_;

    // RPC thread
    std::thread rpc_thread_;

    // Note: this is very important for simulator speed.
    unsigned long long prev_print_t_ = 0;
    // Track speedup from simulator
    std::chrono::time_point<std::chrono::system_clock> prev_print_clock_time_ =
        std::chrono::system_clock::now();
};
