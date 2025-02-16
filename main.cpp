#include <iostream>
#include "socket_server.h"

using namespace std;

int main()
{
    cout << "start" << endl;


    socket_server* server = new socket_server("0.0.0.0", 8080);
    server->listen_and_serve();


    exit(0);
}