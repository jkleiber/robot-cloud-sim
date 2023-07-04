#include "multirotor/multirotor_simulator.h"

bool MultirotorSimulator::Init() { return true; }

bool MultirotorSimulator::Run()
{
    VERIFY(Simulator::Run());

    return true;
}