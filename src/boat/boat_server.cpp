
#include "boat/boat_server.h"

bool BoatServer::Init()
{
    CROW_ROUTE(app_, "/")
    (
        []()
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load("boat_sim.html");
            return page.render(ctx);
        });

    CROW_ROUTE(app_, "/boat/ws")
        .websocket()
        .onopen(
            [&](crow::websocket::connection &conn)
            {
                //
                CROW_LOG_INFO << "new websocket connection";
                conn_ = &conn;
            })
        .onclose(
            [&](crow::websocket::connection &conn, const std::string &reason)
            {
                CROW_LOG_INFO << "websocket connection closed: " << reason;
                conn_ = nullptr;
            })
        .onmessage([&](crow::websocket::connection &conn,
                       const std::string &data, bool is_binary)
                   { std::cout << "Received message: " << data << "\n"; });

    CROW_ROUTE(app_, "/b/<path>")
    (
        [](std::string path)
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load(path);
            return page.render(ctx);
        });

    return true;
}

bool BoatServer::Start()
{
    // Start the server
    app_result_ = app_.port(4001).multithreaded().run_async();

    return true;
}

bool BoatServer::Update(double t)
{
    std::cout << "A\n";
    VERIFY(state_ != nullptr);
    std::cout << "B\n";
    // If the connection is nullptr, return early and don't send anything.
    if (conn_ == nullptr)
    {
        std::cout << "No connection!\n";
        return true;
    }
    std::cout << "C\n";

    // Create a BoatMessage protobuf
    msg::BoatMessage msg;
    msg::BoatMessage::BoatState boat_state;

    std::cout << "D\n";
    msg.set_t(t);

    boat_state.set_lat(state_->lat);
    boat_state.set_lon(state_->lon);
    boat_state.set_yaw(state_->yaw);
    std::cout << "E\n";

    msg.set_allocated_state(&boat_state);
    std::cout << "F\n";
    std::cout << "Sent " << msg.SerializeAsString() << std::endl;
    std::cout << conn_->get_remote_ip() << std::endl;
    conn_->send_binary(msg.SerializeAsString());
    std::cout << "G\n";

    // Encode a RapidJSON message to send to the web UI
    // rapidjson::StringBuffer out_buffer;
    // rapidjson::Writer<rapidjson::StringBuffer> writer(out_buffer);

    // std::string lat_str =

    // writer.StartObject();

    // writer.Key("boat_info");
    // writer.StartObject();
    // writer.Key("lat");
    // writer.String(.c_str());
    // writer.Key("lon");
    // writer.String(right_str.c_str());
    // writer.EndObject();

    // writer.EndObject();

    // Write
    // std::string out_str = out_buffer.GetString();
    // out_str += "\n";

    //
    // conn_->send_text()

    return true;
}