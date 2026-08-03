#ifndef CLIENT_H
#define CLIENT_H

#include <string>

class Client {
public:
    Client();
    ~Client();

    bool connectToServer(const std::string& ip, int port);
    bool sendMessage(const std::string& message);
    std::string receiveMessage();
    void disconnect();

private:
    int socket_fd = -1;
};

#endif
