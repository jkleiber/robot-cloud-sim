
#pragma once

#include <memory>
#include <mutex>
#include <string.h>
#include <unordered_set>

#include <crow.h>
#include <crow/websocket.h>
#include <string.h>

#include "auv/auv_data.h"
#include "auv/auv_msg.pb.h"
#include "auv/auv_rpc_callback.h"
#include "core/logging.h"
#include "core/rpc_manager.h"
#include "core/time.h"

// TODO: make this configurable.
const unsigned long long kWebsocketUpdatePeriod = 500 * kMilli; // sec

#define CROW_ENFORCE_WS_SPEC

class AUVServer
{
public:
    AUVServer(AUVState *state, AUVControl *ctrl) : state_(state), ctrl_(ctrl) {}
    ~AUVServer() {}

    bool Init();
    bool Start();
    bool Update(unsigned long long t);

    // This should be run in another thread in order to allow the rest of the
    // program to run.
    bool RunRpc();

private:
    // Crow app
    crow::SimpleApp app_;
    std::future<void> app_result_;

    // External TCP connection management
    std::shared_ptr<AUVServiceCb> auv_srv_cb_;
    std::shared_ptr<RpcManager<AUVServiceCb>> rpc_;

    // Manage Crow connections over websocket
    std::mutex mtx_;
    std::unordered_set<crow::websocket::connection *> conns_;

    // Websocket timings
    unsigned long long prev_send_t_ = 0;

    // AUV state
    double t_ = 0;
    AUVState *const state_;
    AUVControl *const ctrl_;
    std::string auv_name_;

    bool SendWebsocketData(unsigned long long t);
};