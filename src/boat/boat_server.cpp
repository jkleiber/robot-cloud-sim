
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
        .onopen([&](crow::websocket::connection &conn)
                { CROW_LOG_INFO << "new websocket connection"; })
        .onclose(
            [&](crow::websocket::connection &conn, const std::string &reason)
            { CROW_LOG_INFO << "websocket connection closed: " << reason; })
        .onmessage(
            [&](crow::websocket::connection &conn, const std::string &data,
                bool is_binary)
            {
                std::cout << "Received message: " << data << "\n";
                if (is_binary)
                    conn.send_binary(data);
                else
                    conn.send_text(data);
            });

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
    app_.port(4001).multithreaded().run_async();

    return true;
}