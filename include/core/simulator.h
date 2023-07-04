
#pragma once

#include <memory>
#include <vector>

#include "core/logging.h"
#include "core/server.h"
#include "core/sim_component.h"

class Simulator
{
public:
    Simulator() {}
    ~Simulator() { bool result_ = this->Stop(); }

    bool Init();
    bool Start();
    bool Run();
    bool Stop();

    // Modify simulator private variables
    bool SetDt(unsigned long long dt);
    bool AddComponent(std::shared_ptr<SimComponent> component);

protected:
    // Time (nanoseconds)
    unsigned long long t_;

    // Server for RPC and web interfaces.
    std::shared_ptr<SimServer> server_;

private:
    // Time step (nanoseconds)
    unsigned long long dt_;

    // List of components to start/update during the simulation.
    std::vector<std::shared_ptr<SimComponent>> components_;
};
