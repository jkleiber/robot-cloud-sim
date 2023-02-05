
// System Libraries
#include <iostream>

#include "core/server.h"

int main(int argc, char **argv)
{
    std::cout << "Hello robots\n";

    Server server;
    server.Init();
    server.Start();

    return 0;
}
