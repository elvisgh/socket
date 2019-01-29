#include "server.h"

int main()
{
    ServerSocket server("127.0.0.1", 12333);
    server.run();
    server.closeSocket();

    return 0;
}
