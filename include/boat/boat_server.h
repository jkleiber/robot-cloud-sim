
#pragma once

#include <crow.h>
#include <crow/websocket.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string.h>

#include "boat/boat_data.h"
#include "core/logging.h"

#include "msg/boat_msg.pb.h"

class BoatServer
{
public:
    BoatServer(BoatState *state) : state_(state) {}
    ~BoatServer() {}

    bool Init();
    bool Start();
    bool Update(double t);

private:
    crow::SimpleApp app_;
    std::future<void> app_result_;

    crow::websocket::connection *conn_;

    BoatState *const state_;
};