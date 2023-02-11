
#pragma once

#include <memory>
#include <mutex>
#include <string.h>
#include <unordered_set>

#include <crow.h>
#include <crow/websocket.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <string.h>

#include "boat/boat_data.h"
#include "boat/boat_rpc_callback.h"
#include "core/logging.h"
#include "core/rpc_manager.h"
#include "msg/boat_msg.pb.h"

// TODO: make this configurable.
#define WEBSOCKET_UPDATE_RATE 0.5f // Hz

#define CROW_ENFORCE_WS_SPEC

class BoatServer
{
public:
    BoatServer(BoatState *state, BoatControl *ctrl) : state_(state), ctrl_(ctrl)
    {
    }
    ~BoatServer() {}

    bool Init();
    bool Start();
    bool Update(double t);

    // This should be run in another thread in order to allow the rest of the
    // program to run.
    bool RunRpc();

private:
    // Crow app
    crow::SimpleApp app_;
    std::future<void> app_result_;

    // External TCP connection management
    std::shared_ptr<BoatServiceCb> boat_srv_cb_;
    std::shared_ptr<RpcManager<BoatServiceCb>> rpc_;

    // Manage Crow connections over websocket
    std::mutex mtx_;
    std::unordered_set<crow::websocket::connection *> conns_;

    // Websocket timings
    double prev_send_t_ = -100;

    // Boat state
    BoatState *const state_;
    BoatControl *const ctrl_;
    std::string boat_name_;

    bool SendWebsocketData(double t);
};