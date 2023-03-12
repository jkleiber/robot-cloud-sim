
#include "auv/auv_server.h"

bool AUVServer::Init()
{
    // Serve the main UI on the root page.
    CROW_ROUTE(app_, "/")
    (
        []()
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load("auv_sim.html");
            return page.render(ctx);
        });

    // Websocket route for UI
    CROW_ROUTE(app_, "/auv_ui/ws")
        .websocket()
        .onopen(
            [&](crow::websocket::connection &conn)
            {
                CROW_LOG_INFO << "new websocket connection";
                std::lock_guard<std::mutex> _(mtx_);
                conns_.insert(&conn);
            })
        .onclose(
            [&](crow::websocket::connection &conn, const std::string &reason)
            {
                CROW_LOG_INFO << "websocket connection closed: " << reason;
                std::lock_guard<std::mutex> _(mtx_);
                conns_.erase(&conn);
            })
        .onmessage([&](crow::websocket::connection &conn,
                       const std::string &data, bool is_binary)
                   { std::cout << "Received message: " << data << "\n"; });

    // Arbitrary non-static files can be accessed at this route
    CROW_ROUTE(app_, "/b/<path>")
    (
        [](std::string path)
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load(path);
            return page.render(ctx);
        });

    // RPC setup
    // Build the service callback
    auv_srv_cb_ =
        std::make_shared<AUVServiceCb>(&auv_name_, ctrl_, t_, *state_);

    // Build the RPC manager from the service.
    rpc_ = std::make_shared<RpcManager<AUVServiceCb>>(auv_srv_cb_);

    return true;
}

bool AUVServer::Start()
{
    // Start the server
    app_result_ = app_.port(9001).multithreaded().run_async();

    return true;
}

bool AUVServer::Update(unsigned long long t)
{
    // Update the server time.
    t_ = t;

    // Maintain a specific update rate to the GUI
    unsigned long long delta_send = t - prev_send_t_;
    if (delta_send >= kWebsocketUpdatePeriod)
    {
        VERIFY(SendWebsocketData(t));
    }

    return true;
}

bool AUVServer::RunRpc()
{
    // Set up the RPC manager for managing the RPCs to/from the user.
    VERIFY(rpc_->Init());

    // Run the synchronous RPC server.
    VERIFY(rpc_->Run());

    return true;
}

bool AUVServer::SendWebsocketData(unsigned long long t)
{
    VERIFY(state_ != nullptr);
    VERIFY(ctrl_ != nullptr);

    // Broadcast data to all websocket connections
    for (auto c : conns_)
    {
        // If the connection is nullptr, return early and don't send anything.
        if (c == nullptr)
        {
            conns_.erase(c);
            continue;
        }

        // Create a AUVMessage protobuf
        msg::AUVMessage web_data;

        // Set the time and system state.
        double t_float = static_cast<double>(t) / static_cast<double>(kSec);
        web_data.set_t(t_float);
        web_data.mutable_info()->set_name(auv_name_);
        web_data.mutable_state()->set_t(t_float);
        web_data.mutable_state()->set_lat(state_->lat);
        web_data.mutable_state()->set_lon(state_->lon);
        web_data.mutable_state()->set_yaw(state_->yaw);
        web_data.mutable_state()->set_surge(state_->speed);
        web_data.mutable_state()->set_prop_rpm(state_->prop_rpm);

        // Set the control
        web_data.mutable_ctrl()->set_power(ctrl_->power);
        web_data.mutable_ctrl()->set_rudder(ctrl_->rudder);

        // Send the data as a binary packet.
        c->send_binary(web_data.SerializeAsString());
    }

    // Update the previous send time.
    prev_send_t_ = t;

    return true;
}
