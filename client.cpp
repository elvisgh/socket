#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(12333);
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    char buffer[40];
//    while(true)
    {
    	read(sock, buffer, sizeof(buffer)-1);
    	printf("message fron server: %s\n", buffer);
        sleep(5);
    }

    close(sock);
    return 0;
}
