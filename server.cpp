#include <arpa/inet.h>
#include <map>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

#include "structure.h"

using namespace std;

class ServerSocket
{
public:
    ServerSocket(){};
    ServerSocket(const string& ip, const int& port):m_ip(ip),m_port(port)
    {
        init();
        listenSocket();
    }

    void closeSocket()
    {
        close(m_socket);
    }

    void listenSocket()
    {
        listen(m_socket, 20);//why 20?
    }

    int getSocket()
    {
        return m_socket;
    }

    void transferMessage(int client_sock)
    {
		while(true)
		{
		    printf("client_sock is %d\t", client_sock);
		    char buffer[100];//
	        int signal = read(client_sock, buffer, sizeof(buffer));
	    	if (signal <= 0)
		    {
			    printf("client disconnect.\n");
			    break;
	    	}

            MessageBody tmp;
 	        memcpy(&tmp, buffer, sizeof(tmp));

		    printf("server receive message %s %d %s\n", tmp.destIp, tmp.destPort, tmp.message);
            
            int destSock = m_memo.find(string(tmp.destIp))->second;
            write(destSock, &tmp, sizeof(tmp));
		}
        
    }

    void listenClients()
    {
        struct sockaddr_in client_addr;
        socklen_t client_addr_size = sizeof(client_addr);
        
        int client_sock = accept(m_socket, (struct sockaddr*)&client_addr, &client_addr_size);
        if (client_sock > 0)
        {
            const char* tmp_ip = inet_ntoa(client_addr.sin_addr);
            string client_ip = tmp_ip;
            int client_port = ntohs(client_addr.sin_port);
		    printf("client %d connect sucessfully, ip : %s port : %d\n", 
				client_sock, client_ip.c_str(), client_port);

            m_memo.insert(pair<string, int>(client_ip,client_sock));
            printf("client %d added in memo.\n", client_sock);

		  	std::thread t = std::thread(&ServerSocket::transferMessage, this, client_sock);
		    t.detach();
        }
		else
		{
			printf("are you connecting again???\n");
			sleep(2);
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

    map<string, int> m_memo;
};

int main()
{  
    ServerSocket server_socket("127.0.0.1", 12333); 
    
    // while(true)
    //{
    //    std::thread serverThreads[2];
    //    for (int i = 0; i < 2; ++i)
    //    {
    //        serverThreads[i] = std::thread(&ServerSocket::listenClients, server_socket);
    //    }

    //    for (int i = 0; i < 2; ++i)
    //    {
    //        serverThreads[i].detach();
    //    }
    //}

    while(true)
    {
        //keep server running
		server_socket.listenClients();
    }

    server_socket.closeSocket();
    return 0;
}
