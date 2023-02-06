
#pragma once

#include <crow.h>
#include <string.h>

class BoatServer
{
public:
    BoatServer() {}
    ~BoatServer() {}

    bool Init();
    bool Start();

private:
    crow::SimpleApp app_;
};