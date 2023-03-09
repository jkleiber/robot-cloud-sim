
#include "boat/boat_simulator.h"

bool BoatSimulator::Init()
{
    // TODO: make dt configurable
    t_ = 0;
    boat_data_.sim_info.dt = 100 * kMicro;

    // Initialize server
    server_ = std::make_shared<BoatServer>(&boat_data_.state, &boat_data_.ctrl);
    VERIFY(server_->Init());

    // Initialize boat dynamics
    boat_sys_ = std::make_shared<BoatDynamics>(
        &boat_data_.state, &boat_data_.ctrl, boat_data_.sim_info.dt);
    VERIFY(boat_sys_->Init());

    return true;
}

bool BoatSimulator::Run()
{
    VERIFY(server_ != nullptr);
    VERIFY(boat_sys_ != nullptr);

    // Start up the server
    VERIFY(server_->Start());

    // Run the RPC management in a separate thread.
    std::thread rpc_thread(&BoatServer::RunRpc, server_.get());

    // Run the dynamics
    while (true)
    {
        // Update the boat physics.
        VERIFY(boat_sys_->Update());

        // Update the server.
        VERIFY(server_->Update(t_));

        // Update the time
        t_ += boat_data_.sim_info.dt;

        // Print the status every second (in sim time).
        // Note: adding this print slows the simulation down enough for the
        // websockets to work well. The print rate and the dt of the simulation
        // are related to the user experience in terms of timing.
        //
        // As the simulator becomes more complex, this may need to be adjusted
        // in order to maintain approximately 10x speed performance in the sim.
        // A better way to do this would be to maintain tighter control on the
        // loop timing, but this is difficult to do across operating systems.
        if ((t_ - prev_print_t_) > kPrintPeriod)
        {
            auto cur_clock_time = std::chrono::system_clock::now();
            auto elapsed_clock_time =
                std::chrono::duration_cast<std::chrono::nanoseconds>(
                    cur_clock_time - prev_print_clock_time_);
            prev_print_clock_time_ = cur_clock_time;

            // Calculate speed up as (sim time elapsed / actual time elapsed).
            double speedup_pct =
                static_cast<double>(kPrintPeriod) /
                static_cast<double>(elapsed_clock_time.count());

            std ::cout << "t=" << t_ / kSec << " | " << std::fixed
                       << std::setprecision(2) << speedup_pct << "x\n";
            prev_print_t_ = t_;
        }
    }

    rpc_thread.join();

    return true;
}