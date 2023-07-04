#pragma once

#include "core/sim_component.h"

class SimServer : public SimComponent
{
public:
    SimServer(std::string name) : SimComponent(name) {}
    ~SimServer() {}

    virtual bool Init();
    virtual bool Start();
    virtual bool Update(unsigned long long t);

    // This should be run in another thread in order to allow the rest of the
    // program to run.
    virtual bool RunRpc() { return true; }
};
