
#include "core/server.h"

void Server::Init()
{
    CROW_ROUTE(app_, "/")
    (
        []()
        {
            crow::mustache::context ctx;
            auto page = crow::mustache::load("crow_test.html");
            return page.render(ctx);
            // return "Hello world";
        });
}

void Server::Start()
{
    // Start the server
    app_.port(4001).multithreaded().run();
}