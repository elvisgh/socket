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
    MySocket(const string& addr, const int& port)
    {
        m_addr = addr;
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

private:
    void init()
    {    
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
        struct sockaddr_in socket_addr;
        memset(&socket_addr, 0, sizeof(socket_addr));
        socket_addr.sin_family = AF_INET;
        socket_addr.sin_addr.s_addr = inet_addr(m_addr.data());
        socket_addr.sin_port = htons(m_port);
        bind(m_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr));
    }

private:
    string m_addr;
    int m_port;
    int m_socket;
};

void response(MySocket &mySocket)
{
    char str[] = "hello";
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sock = accept(mySocket.getSocket(), (struct sockaddr*)&client_addr, &client_addr_size);

    printf("%d\n", client_sock);

    if (client_sock != 0)
    {
        printf("receive client request\n");
    }
            
    write(client_sock, str, sizeof(str));

    close(client_sock);
}

int main()
{
//    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//   
//    struct sockaddr_in server_addr;
//    memset(&server_addr, 0, sizeof(server_addr));
//    server_addr.sin_family = AF_INET;
//    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//    server_addr.sin_port = htons(12333);
//    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
//
//    listen(server_socket, 20);
    
    MySocket server_socket = MySocket("127.0.0.1", 12333);

//    struct sockaddr_in client_addr;
//    socklen_t client_addr_size = sizeof(client_addr);
//    int client_sock = accept(server_socket.getSocket(), (struct sockaddr*)&client_addr, &client_addr_size);
    
    
    while(true)
    {
        response(server_socket);

//thread is wrong, need more pratice 2018/09/26 23:54 elvis

        // std::thread serverThreads[10];
        // for (int i = 0; i < 10; ++i)
        // {
        //     serverThreads[i] = std::thread(response, server_socket);
        // }

        // for (int i = 0; i < 10; ++i)
        // {
        //     serverThreads[i].join();
        // }


        // char str[] = "hello";
        // struct sockaddr_in client_addr;
        // socklen_t client_addr_size = sizeof(client_addr);
        // int client_sock = accept(server_socket.getSocket(), (struct sockaddr*)&client_addr, &client_addr_size);

        // printf("%d\n", client_sock);

        // if (client_sock != 0)
        // {
        //     printf("receive client request\n");
        // }
    	
        // write(client_sock, str, sizeof(str));

        // close(client_sock);
    }

//    close(client_sock);
//    close(server_socket);
    server_socket.closeSocket();
    return 0;
}
