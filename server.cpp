#include "server.h"
#include <map>
#include <thread>
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
std::map<std::string, int> users;
std::string send1;
std::string send2;
bool Server::start(int port)
{
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
        return false;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd,
            reinterpret_cast<sockaddr*>(&address),
            sizeof(address)) < 0)
        return false;

    if(listen(server_fd, SOMAXCONN) < 0)
        return false;

    std::cout << "Listening on port " << port << '\n';

    return true;
}

void handleclient(int client_fd){
    std::string username;
while(true){
        char buffer[1024]{};

    ssize_t bytes =
        recv(client_fd,
             buffer,
             sizeof(buffer)-1,
             0);
    if(bytes <= 0)
{
    if(bytes == 0)
        std::cout << "Client disconnected!\n";
    else
        std::cout << "Error occurred!\n";

    break;
}

buffer[bytes] = '\0';
    std::string message(buffer);
    if(message.rfind("LOGIN ", 0) == 0)
{
    username = message.substr(6);

    users[username] = client_fd;

    std::cout << username << " logged in.\n";
}
else if(message.rfind("Find ", 0) == 0)
{
    std::string find = message.substr(5);

    auto it = users.find(find);
        if (it != users.end()){
            send(client_fd, "OK", 2, 0);
            std::cout << "OK \n";
    }
    else {
        send(client_fd, "NO", 2, 0);
        std::cout << "NO \n";
    }
}
else if(message.rfind("U ", 0) == 0)
{
    std::string data = message.substr(2);
    size_t pos = data.find(' ');
    std::string resiver = data.substr(0, pos); 
    std::string text = data.substr(pos + 1);
    std::string packet = username + ": " + text;
      auto it = users.find(resiver);

    if(it != users.end())
    {
        int receiver_socket = it->second;

        send(receiver_socket,
             packet.c_str(),
             packet.size(),
             0);
    }
}
}
    users.erase(username);
    close(client_fd);
}

void Server::run()
{
    while(true){
    sockaddr_in client{};
    socklen_t len = sizeof(client);
    int client_fd =
        accept(server_fd,
               reinterpret_cast<sockaddr*>(&client),
               &len);
    if(client_fd < 0){
            std::cout << "Accept failed..\n";
        continue;}
    std::thread t(handleclient, client_fd);
    t.detach();
}
}


void Server::stop()
{
    if(server_fd != -1)
    {
        close(server_fd);
        server_fd = -1;
    }
}
