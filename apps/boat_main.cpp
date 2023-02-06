
// System Libraries
#include <iostream>

#include "boat/boat_simulator.h"
#include "core/logging.h"

int main(int argc, char **argv)
{
    BoatSimulator sim;
    VERIFY(sim.Init());
    VERIFY(sim.Run());

    return 0;
}
