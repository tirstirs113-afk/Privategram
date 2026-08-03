#include "server.h"

int main()
{
    Server server;

    if(!server.start(8080))
        return 1;

    server.run();
    server.stop();

    return 0;
}
