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

    int readMessage(const int& sock)
    {
        printf("sock is %d, readMessage is running...\n", sock);
        
        char buffer[100];
        int ret = read(sock, buffer, sizeof(buffer));//read is ok?
        MessageBody tmp;
 	    memcpy(&tmp, buffer, sizeof(tmp));
        printf("receive message %s from sock %d.\n", tmp.message, sock);

        return ret;
    }

    int writeMessage(string ip, int port, string message)
    {
        AddressUnit addr;
        addr.ip = ip;
        addr.port = port;

        MessageBody tmp;
        strcpy(tmp.message, "hahahahha");
        
        int destSock = m_memo.find(addr)->second;
        write(destSock, &tmp, sizeof(tmp));
    }

    void transferMessage(int client_sock)
    {
        int signal = 0;

		while(true)
		{
		    printf("client_sock is %d\t", client_sock);
		    
            char buffer[100];//
	        signal = read(client_sock, buffer, sizeof(buffer));
	    	if (signal <= 0)
		    {
			    printf("no message received, thread shutdown.\n");
			    break;
	    	}

            MessageBody tmp;
 	        memcpy(&tmp, buffer, sizeof(tmp));

		    printf("server receive message %s %d %s\n", tmp.destIp, tmp.destPort, tmp.message);
            

            AddressUnit addr;
            addr.ip = string(tmp.destIp);
            addr.port = tmp.destPort;

			if (m_memo.find(addr) != m_memo.end())
			{
            	int destSock = m_memo.find(addr)->second;
				printf("transfer message to %d \n", destSock);
			
				struct tcp_info info;
				int len = sizeof(info);
				getsockopt(destSock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&len);
				if (info.tcpi_state == TCP_ESTABLISHED)
				{
					write(destSock, &tmp, sizeof(tmp)); //if clients shutdown, buffers overflow, dump!
				}
				else
				{
					printf("dest sock %d disconnect, can not write message.\n", destSock);
					m_memo.erase(addr);
				}
			}
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

            AddressUnit addr;
            addr.ip = client_ip;
            addr.port = client_port;
            m_memo.insert(pair<AddressUnit, int>(addr, client_sock));
            printf("client %d added in memo.\n", client_sock);

		  	std::thread t = std::thread(&ServerSocket::transferMessage, this, client_sock);
		    t.detach();
        }
//		else
//		{
//			printf("are you connecting again???\n");
//			sleep(2);
//		}
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

//    map<string, int> m_memo;
    map<AddressUnit, int> m_memo;
};

int main()
{  
    ServerSocket server_socket("127.0.0.1", 12333); 
    
    while(true)
    {
        //keep server running
		server_socket.listenClients();
    }

    server_socket.closeSocket();
    return 0;
}
