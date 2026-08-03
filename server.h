#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
public:
    bool start(int port);
    void run();
    void stop();

private:
    int server_fd;
};
#endif
