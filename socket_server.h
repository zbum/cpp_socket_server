#pragma once

#include <iostream>
using namespace std;

class socket_server
{
private:
    string addr_v4;
    int port;
public:
    socket_server(string addr_v4, int port);
    ~socket_server();
    int listen_and_serve();
};
