#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <thread>

using namespace std;

class MySocket
{
public:
    MySocket(){};
    MySocket(const string& ip, const int& port)
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

    void response()
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        int client_sock = accept(m_socket, (struct sockaddr*)&client_addr, &client_addr_size);

        printf("%d\t", client_sock);

        if (client_sock != -1)
        {
            printf("receive client request\n");
        }

        char request[40];
        read(client_sock, request, sizeof(request)-1);
        printf("message fron client: %s\n", request);
            
        char str[] = "hello, this is server";
        write(client_sock, str, sizeof(str));

        close(client_sock);
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
    MySocket server_socket("127.0.0.1", 12333); 
    
    // while(true)
    {
        std::thread serverThreads[2];
        for (int i = 0; i < 2; ++i)
        {
            serverThreads[i] = std::thread(&MySocket::response, server_socket);
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
