#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <thread>

#include "structure.h"

using namespace std;

class ClinetInterface
{
public:
    ClinetInterface(){};
    ClinetInterface(const string& ip, const int& port)
    {
        m_ip = ip;
        m_port = port;

        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        struct sockaddr_in socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = inet_addr(ip.data());
        socket_addr.sin_port = htons(port);
        bind(m_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    }

    int connectServer(const string& serverIP, const int& serverPort)
    {
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(serverIP.data());
        server_addr.sin_port = htons(serverPort);

        return connect(m_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    int writeMessage(const string& request)
    {
        int result = -1;

		MessageBody writeMB;
		writeMB.message = 19930417;

		result = write(m_socket, &writeMB, sizeof(writeMB));

        return result;
    }

    void readMessage()
    {
        char buffer[40];
    	
        if (read(m_socket, buffer, sizeof(buffer)-1))
        {
			MessageBody readMB;
			memcpy(&readMB, buffer, sizeof(buffer));
			printf("receive message from server : %d\n", readMB.message);
        }
        else
        {
            //read failure
            printf("xxxxxxxxxxxxxx\n");
        }
    }

    void closeSocket()
    {
        close(m_socket);
    }

private:
    string m_ip;
    int m_port;
    int m_socket;
};

int main()
{
    ClinetInterface clientDemo("127.0.0.1", 12334);
    int signal = clientDemo.connectServer("127.0.0.1", 12333);
    if (signal > 0)
    {
        printf("connect server sucessfully.\n");
    }

    std::thread clientThreads[1];
    int client_num = 1;

    string request = "hello, this is client";
    for (int i = 0; i < client_num; ++i)
    {
        clientThreads[i] = std::thread(&ClinetInterface::writeMessage, clientDemo, request);
    }

    for (int i = 0; i < client_num; ++i)
    {
        clientThreads[i].detach();
    }

    std::thread clientThreads2 = thread(&ClinetInterface::readMessage, clientDemo);
    clientThreads2.detach();

    while(true)
    {
        //keep client running
    }

    clientDemo.closeSocket();
    return 0;
}
