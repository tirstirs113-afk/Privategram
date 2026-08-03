#include "client.h"
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

Client::Client()
{
    socket_fd = -1;
}

Client::~Client()
{
    disconnect();
}

bool Client::connectToServer(const std::string& ip, int port)
{
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd < 0)
        return false;

    sockaddr_in server{};

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0)
        return false;

    if(connect(socket_fd,
               reinterpret_cast<sockaddr*>(&server),
               sizeof(server)) < 0)
        return false;

    return true;
}

bool Client::sendMessage(const std::string& message)
{
    return send(socket_fd,
                message.c_str(),
                message.size(),
                0) >= 0;
}

std::string Client::receiveMessage()
{
    char buffer[1024]{};

    recv(socket_fd,
         buffer,
         sizeof(buffer)-1,
         0);  

    return buffer;
}

void Client::disconnect()
{
    if(socket_fd != -1)
    {
        close(socket_fd);
        socket_fd = -1;
    }
}
