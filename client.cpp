#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <thread>

using namespace std;

int connectServer(const char* serverAddr, const int& serverPort)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    // server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // server_addr.sin_port = htons(12333);
    server_addr.sin_addr.s_addr = inet_addr(serverAddr);
    server_addr.sin_port = htons(serverPort);
    
    int connectSignal = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    return connectSignal;
}

void createClient()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(12333);
    int signal = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if (signal == 0)
    {
        printf("connect server sucessfully\n");
    }

    char request[] = "hello, this is client";
    write(sock, request, sizeof(request));
    
    char buffer[40];
//    while(true)
    {
    	read(sock, buffer, sizeof(buffer)-1);
    	printf("message fron server: %s\n", buffer);
        sleep(5);
    }

    close(sock);
}

int main()
{
//     int sock = socket(AF_INET, SOCK_STREAM, 0);
    
//     struct sockaddr_in server_addr;
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     server_addr.sin_port = htons(12333);
//     connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
//     char buffer[40];
// //    while(true)
//     {
//     	read(sock, buffer, sizeof(buffer)-1);
//     	printf("message fron server: %s\n", buffer);
//         sleep(5);
//     }

//     close(sock);

    std::thread clientThreads[1];
    int client_num = 1;

    for (int i = 0; i < client_num; ++i)
    {
        clientThreads[i] = std::thread(createClient);
    }

    for (int i = 0; i < client_num; ++i)
    {
        clientThreads[i].join();
    }

    return 0;
}