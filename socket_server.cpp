#include "socket_server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#define BUFFER_SIZE 1024

socket_server::socket_server(string addr_v4, int port)
{
    cout << "ip:" << addr_v4 << ", port" << port << endl;
    this->addr_v4 = addr_v4;
    this->port = port;
}

socket_server::~socket_server()
{
}

void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    while(true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0 ) {
            cout << "Client disconnected" << endl;
            break;
        }
        std::cout << "Received: " << buffer << std::endl;

        string response = "Echo: " + string(buffer);
        send(client_socket, response.c_str(), response.length(), 0);
    }

    close(client_socket);
    std::cout << "Client disconnected" << std::endl;
    return nullptr;
}

int socket_server::listen_and_serve()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        return 1;
    }

    int opt = 1;
    if ( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 ) {
        perror("setsockopt failed.");
        close(server_fd);
        return 1;
    }

    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(this->port);

    if (inet_pton(AF_INET, this->addr_v4.c_str(), &address.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address / Address not supported : %s \n", this->addr_v4.c_str());
        close(server_fd);
        return 1;
    } 
    
    if ( ::bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1) {
        perror( "Bind failed.");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) == -1 ) {
        perror("Listen failed.");
        close(server_fd);
        return 1;
    }

    std::cout << "Server listening on port " << this->port << std::endl;

    int client_socket;
    socklen_t addr_len = sizeof(address);
    while (true) {
        client_socket = accept(server_fd, (struct sockaddr*)&address, &addr_len) ;
        if ( client_socket == -1) {
            perror("Accept failed");
            continue;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, client_ip, INET_ADDRSTRLEN);
        int client_port = ntohs(address.sin_port);

        std::cout << "New client connected: " << client_ip << ":" << client_port << std::endl;

        int* client_ptr = (int *)malloc(sizeof(int));
        *client_ptr = client_socket;

        pthread_t thread_id;
        pthread_create(&thread_id, nullptr, handle_client, client_ptr);
        pthread_detach(thread_id);

    }

    close(server_fd);    
    return 0;
}

