
#include "boat/boat_server.h"

bool BoatServer::Init()
{
    CROW_ROUTE(app_, "/")
    (
        []()
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load("crow_test.html");
            return page.render(ctx);
        });

    CROW_ROUTE(app_, "/boat/<path>")
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