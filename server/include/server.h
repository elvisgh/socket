#include <arpa/inet.h>
#include <map>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>

#include "structure.h"

using namespace std;

class ServerSocket
{
public:
    ServerSocket() = delete;
    ServerSocket(const string& ip, const int& port);

    void closeSocket();

    void listenSocket();

    int getSocket();

    int readMessage(const int& socket_fd);
//    int writeMessage(const int& socket_fd);

    void handleMessage(const int& socket_fd);

    void listenClients();

    void run();

private:
    void configServer();

private:
    string m_ip;
    int m_port;
    int m_socket;

    map<int, int> m_memo;
};
