
#pragma once

#include "core/simulator.h"

class MultirotorSimulator : public Simulator
{
public:
    MultirotorSimulator() {}
    ~MultirotorSimulator() {}

    bool Init();
    bool Run();

private:
};
