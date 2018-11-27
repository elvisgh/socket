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
    ClinetInterface(const string& serverIP, const int& serverPort)
    {
        m_ip = serverIP;
        m_port = serverPort;

        if (connectServer() == 0)
        {
            printf("connect server sucessfully\n");
        }
        else
        {
            //connect failure
        }
    }

    int connectServer()
    {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
    
        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = inet_addr(m_ip.data());
        server_addr.sin_port = htons(m_port);

        return connect(m_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    }

    int writeMessage(const string& request)
    {
        int result = -1;

		MessageBody writeMB;
		writeMB.message = 55555;

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
    ClinetInterface clientDemo("127.0.0.1", 12333);

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
