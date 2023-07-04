
// System Libraries
#include <iostream>

#include "boat/boat_simulator.h"
#include "core/logging.h"

int main(int argc, char **argv)
{
    // Initialize and start the simulator.
    BoatSimulator sim;
    VERIFY(sim.Init());
    VERIFY(sim.Start());

    // Run the simulator.
    while (true)
    {
        VERIFY(sim.Run());
    }

    // Stop the simulator.
    VERIFY(sim.Stop());

    return 0;
}
