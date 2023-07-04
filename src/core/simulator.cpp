#include "core/simulator.h"

bool Simulator::Init() { return true; }

bool Simulator::Start()
{
    // Start all the components.
    for (std::shared_ptr<SimComponent> c : components_)
    {
        VERIFY(c != nullptr);
        VERIFY(c->Start());
    }

    // Start the server.
    VERIFY(server_->Start());

    return true;
}

bool Simulator::Run()
{
    // Run the update function for each component.
    for (std::shared_ptr<SimComponent> c : components_)
    {
        VERIFY(c != nullptr);
        VERIFY(c->Update(t_));
    }

    // Update the server.
    VERIFY(server_->Update(t_));

    // Update the time.
    t_ += dt_;

    return true;
}

bool Simulator::Stop() { return true; }

bool Simulator::SetDt(unsigned long long dt)
{
    dt_ = dt;
    return true;
}

bool Simulator::AddComponent(std::shared_ptr<SimComponent> component)
{
    VERIFY(component != nullptr);

    // Ensure name is unique.
    for (auto c : components_)
    {
        // Fail if the name is not unique.
        VERIFY(c->GetName() != component->GetName());
    }

    // Add the valid component to the list.
    components_.push_back(component);

    return true;
}