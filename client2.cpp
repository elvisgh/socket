#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "structure.h"

using namespace std;

class ClinetInterface
{
public:
    ClinetInterface(){};
    ClinetInterface(const string& ip, const int& port):m_ip(ip),m_port(port)
    {
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

    int writeMessage()
    {
        int result = -1;

		MessageBody writeMB;
		strcpy(writeMB.destIp, "127.0.0.1");
		writeMB.destPort = 12334;
		strcpy(writeMB.message, "hello, i am 12335");

		printf("xxxxxxxxx%s %d\n", writeMB.destIp, writeMB.destPort);

		char buffer[100];
		memcpy(buffer, &writeMB, sizeof(writeMB));
		result = write(m_socket, buffer, sizeof(buffer));

        return result;
    }

    void readMessage()
    {
        char buffer[100];
    	
        if (read(m_socket, buffer, sizeof(buffer)-1))
        {
			MessageBody readMB;
			memcpy(&readMB, buffer, sizeof(readMB));
			printf("receive message from server : %s\n", readMB.message);
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
    ClinetInterface clientDemo("127.0.0.1", 12335);
    int signal = clientDemo.connectServer("127.0.0.1", 12333);
    while(true)
    {
        //keep client running
    	if (signal == 0)
    	{
        	printf("connect server sucessfully.\n");
			clientDemo.readMessage();
			sleep(5);
    	}
		else
		{
			printf("connect failure, reconnecting\n");
			signal = clientDemo.connectServer("127.0.0.1", 12333);
			sleep(2);
		}
    }

    clientDemo.closeSocket();
    return 0;
}
