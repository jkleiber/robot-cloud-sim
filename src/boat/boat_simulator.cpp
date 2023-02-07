
#include "boat/boat_simulator.h"

bool BoatSimulator::Init()
{
    // TODO: make configurable
    t_ = 0;
    boat_data_.sim_info.dt = 0.01;

    // Initialize server
    server_ = std::make_shared<BoatServer>(&boat_data_.state);
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
    std::cout << "YEEE\n";
    VERIFY(server_->Start());
    std::cout << "YEET\n";

    // Run the dynamics
    while (true)
    {
        std::cout << "YEEE\n";
        // VERIFY(boat_sys_->Update())
        VERIFY(server_->Update(t_));

        std::cout << t_ << " sec\n";

        // Update the time
        t_ += boat_data_.sim_info.dt;
    }

    return true;
}