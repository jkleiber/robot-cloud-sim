
#include "boat/boat_simulator.h"

bool BoatSimulator::Init()
{
    // TODO: make configurable
    boat_data_.sim_info.dt = 0.01;

    // Initialize server
    server_ = std::make_shared<BoatServer>();
    VERIFY(server_->Init());

    // Initialize boat dynamics
    boat_sys_ = std::make_shared<BoatDynamics>(&boat_data_.state,
                                               boat_data_.sim_info.dt);
    VERIFY(boat_sys_->Init());

    return true;
}

bool BoatSimulator::Run()
{
    VERIFY(server_ != nullptr);
    VERIFY(boat_sys_ != nullptr);

    // Start up the server
    VERIFY(server_->Start());

    // Run the dynamics
    while (1)
    {
        // VERIFY(boat_sys_->Update())
    }

    return true;
}