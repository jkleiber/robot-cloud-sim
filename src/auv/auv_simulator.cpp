
#include "auv/auv_simulator.h"

bool AUVSimulator::Init()
{
    // TODO: make dt configurable
    t_ = 0;
    auv_data_.sim_info.dt = 100 * kMicro;

    // Initialize server
    server_ = std::make_shared<AUVServer>(&auv_data_.state, &auv_data_.ctrl);
    VERIFY(server_->Init());

    // Initialize auv dynamics
    auv_sys_ = std::make_shared<AUVDynamics>(&auv_data_.state, &auv_data_.ctrl,
                                             auv_data_.sim_info.dt);
    VERIFY(auv_sys_->Init());

    return true;
}

bool AUVSimulator::Run()
{
    VERIFY(server_ != nullptr);
    VERIFY(auv_sys_ != nullptr);

    // Start up the server
    VERIFY(server_->Start());

    // Run the RPC management in a separate thread.
    std::thread rpc_thread(&AUVServer::RunRpc, server_.get());

    // Run the dynamics
    while (true)
    {
        // Update the auv physics.
        VERIFY(auv_sys_->Update());

        // Update the server.
        VERIFY(server_->Update(t_));

        // Update the time
        t_ += auv_data_.sim_info.dt;

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