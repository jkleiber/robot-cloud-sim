
#include "core/sim_component.h"

bool SimComponent::Init() { return true; }

bool SimComponent::Update(unsigned long long t) { return true; }

bool SimComponent::Start() { return true; }

bool SimComponent::Stop() { return true; }

std::string SimComponent::GetName() { return this->name_; }