#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <thread>

#include "structure.h"

using namespace std;

class ServerSocket
{
public:
    ServerSocket(){};
    ServerSocket(const string& ip, const int& port)
    {
        m_ip = ip;
        m_port = port;
        init();
        listenSocket();
    }

    void closeSocket()
    {
        close(m_socket);
    }

    void listenSocket()
    {
        listen(m_socket, 20);
    }

    int getSocket()
    {
        return m_socket;
    }

    void transferMessage()
    {
    }

    void listenClients()
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        
        int client_sock = accept(m_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_sock > 0)
        {
		    printf("client %d connect sucessfully, ip : %s port : %d\n", client_sock, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

            if (ntohs(client_addr.sin_port) != 12334)
            {
                MessageBody tmp;
                tmp.message = 13149;
                int destSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                struct sockaddr_in destSock_addr;
                destSock_addr.sin_family = AF_INET;
                destSock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
                destSock_addr.sin_port = htons(12334);
                write(destSock, &tmp, sizeof(tmp));
                return;
            }

    	    char buffer[40];//
		    read(client_sock, buffer, sizeof(buffer));
   		    MessageBody writeMB;
   		    memcpy(&writeMB, buffer, sizeof(writeMB));
   		    printf("receive client message: %d\n", writeMB.message);    

		    writeMB.message += 1;
   		    write(client_sock, &writeMB, sizeof(writeMB));
        }
    }

private:
    void init()
    {    
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
        struct sockaddr_in socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = inet_addr(m_ip.data());
        socket_addr.sin_port = htons(m_port);
        bind(m_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    }

private:
    string m_ip;
    int m_port;
    int m_socket;
};

int main()
{  
    ServerSocket server_socket("127.0.0.1", 12333); 
    
    // while(true)
    {
        std::thread serverThreads[2];
        for (int i = 0; i < 2; ++i)
        {
            serverThreads[i] = std::thread(&ServerSocket::listenClients, server_socket);
        }

        for (int i = 0; i < 2; ++i)
        {
            serverThreads[i].detach();
        }
    }

    while(true)
    {
        //keep server running
    }

    server_socket.closeSocket();
    return 0;
}
