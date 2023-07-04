#pragma once

#include <string>

class SimComponent
{
public:
    SimComponent(std::string name) : name_(name) {}
    ~SimComponent() {}

    virtual bool Init();
    virtual bool Update(unsigned long long t);
    virtual bool Start();
    virtual bool Stop();

    // Name
    std::string GetName();

protected:
    std::string name_;
};
