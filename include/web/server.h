
#pragma once

#include <crow.h>

class Server
{
public:
    Server() {}
    ~Server() {}

    void Start();

private:
    crow::SimpleApp app_;
};