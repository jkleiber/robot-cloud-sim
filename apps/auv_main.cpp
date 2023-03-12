
// System Libraries
#include <iostream>

#include "auv/auv_simulator.h"
#include "core/logging.h"

int main(int argc, char **argv)
{
    AUVSimulator sim;
    VERIFY(sim.Init());
    VERIFY(sim.Run());

    return 0;
}
