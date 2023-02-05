
#include "web/server.h"

void Server::Start()
{
    CROW_ROUTE(app_, "/")([]() { return "Hello world"; });

    // Start the server
    app_.port(4001).multithreaded().run();
}