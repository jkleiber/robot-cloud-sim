
// System Libraries
#include <iostream>
#include <unistd.h>

#include "web/server.h"

int main(int argc, char **argv)
{
    std::cout << "Hello robots\n";

    Server server;
    server.Start();

    return 0;
}
