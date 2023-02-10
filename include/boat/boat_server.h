
#pragma once

#include <memory>
#include <mutex>
#include <unordered_set>

#include <crow.h>
#include <crow/websocket.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string.h>

#include "boat/boat_data.h"
#include "core/logging.h"
#include "msg/boat_msg.pb.h"

// TODO: make this configurable.
#define WEBSOCKET_UPDATE_RATE 0.5f // Hz

#define CROW_ENFORCE_WS_SPEC

class BoatServer
{
public:
    BoatServer(BoatState *state) : state_(state) {}
    ~BoatServer() {}

    bool Init();
    bool Start();
    bool Update(double t);

private:
    // Crow app
    crow::SimpleApp app_;
    std::future<void> app_result_;

    // External TCP connection management
    // TODO

    // Manage Crow connections over websocket
    std::mutex mtx_;
    std::unordered_set<crow::websocket::connection *> conns_;

    // Websocket timings
    double prev_send_t_ = -100;

    // Boat state
    BoatState *const state_;

    bool SendWebsocketData(double t);
};