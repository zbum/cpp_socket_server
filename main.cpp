#include <iostream>
#include "socket_server.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3 ) {
        fprintf(stderr, "Usage: %s <ip address> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    string server_id = string(argv[1]);
    int port = atoi(argv[2]);

    cout << "starting..." << endl;


    socket_server* server = new socket_server(server_id, port);
    server->listen_and_serve();


    exit(0);
}