
#include "boat/boat_server.h"

bool BoatServer::Init()
{
    // Serve the main UI on the root page.
    CROW_ROUTE(app_, "/")
    (
        []()
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load("boat_sim.html");
            return page.render(ctx);
        });

    // Websocket route for UI
    CROW_ROUTE(app_, "/boat_ui/ws")
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

    // Set up an RPC manager for managing the RPCs to/from the user.
    VERIFY(user_rpc_.Init());

    return true;
}

bool BoatServer::Start()
{
    // Start the server
    app_result_ = app_.port(9001).multithreaded().run_async();

    // Run the RPC manager asynchronously
    VERIFY(user_rpc_.AsyncRun());

    return true;
}

bool BoatServer::Update(double t)
{
    // Maintain a specific update rate to the GUI
    double delta_send = t - prev_send_t_;
    if (delta_send >= (1 / WEBSOCKET_UPDATE_RATE))
    {
        VERIFY(SendWebsocketData(t));
    }

    return true;
}

bool BoatServer::SendWebsocketData(double t)
{
    VERIFY(state_ != nullptr);

    // Broadcast data to all websocket connections
    for (auto c : conns_)
    {
        // If the connection is nullptr, return early and don't send anything.
        if (c == nullptr)
        {
            conns_.erase(c);
            continue;
        }

        // Create a BoatMessage protobuf
        msg::BoatMessage web_data;

        // Set the time and state for the system.
        web_data.set_t(t);
        web_data.mutable_state()->set_lat(state_->lat);
        web_data.mutable_state()->set_lon(state_->lon);
        web_data.mutable_state()->set_yaw(state_->yaw);

        // Send the data as a binary packet.
        c->send_binary(web_data.SerializeAsString());
    }

    // Update the previous send time.
    prev_send_t_ = t;

    return true;
}