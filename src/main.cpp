#include "client/client.h"
//#include "cmdline.h"
#include "server/server.h"
#include "world.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    /*
    cmdline::parser arger;
    arger.add<bool>("server", '\0', "only start server", false, false);
    arger.parse_check(argc, argv);
    */
    Server server;
    Client client(server.getLocalWorldPtr());
    client.mainloop();
    server.shutdown();
}