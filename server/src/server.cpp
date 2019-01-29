#include "server.h"

ServerSocket::ServerSocket(const string& ip, const int& port):m_ip(ip),m_port(port)
{
    configServer();
    listenSocket();
}

void ServerSocket::closeSocket()
{
    close(m_socket);
}

void ServerSocket::listenSocket()
{
    listen(m_socket, 20);//why 20?
}

int ServerSocket::getSocket()
{
    return m_socket;
}

int ServerSocket::readMessage(const int& socket_fd)
{
    printf("sock is %d, readMessage is running...\n", socket_fd);

    char buffer[100];
    int ret = read(socket_fd, buffer, sizeof(buffer));//read is ok?
    MessageBody tmp;
    memcpy(&tmp, buffer, sizeof(tmp));
    printf("receive message %s from sock %d.\n", tmp.message, socket_fd);

    return ret;
}

void ServerSocket::handleMessage(const int& socket_fd)
{
    int signal = 0;

    while(true)
    {
        printf("client_sock is %d\t", socket_fd);
    
        char buffer[100];//
        signal = read(socket_fd, buffer, sizeof(buffer));
        if (signal <= 0)
        {
            printf("no message received, thread shutdown.\n");
            break;
        }

        MessageBody tmp;
        memcpy(&tmp, buffer, sizeof(tmp));

        printf("server receive message %d %d %s\n", tmp.source, tmp.dest, tmp.message);
        m_memo.insert(pair<int, int>(tmp.source, socket_fd));
    
        if (m_memo.find(tmp.dest) != m_memo.end())
        {
            int destSock = m_memo.find(tmp.dest)->second;
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
                printf("dest sock %d disconnect, can not write message.\n", tmp.dest);
                close(destSock);//should i close this dumped fd?
                m_memo.erase(tmp.dest);
            }
        }
        else
        {
            printf("no that destination.\n");
        }
    }
}

void ServerSocket::listenClients()
{
    struct sockaddr_in client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    int client_sock = accept(m_socket, (struct sockaddr*)&client_addr, &client_addr_size);
    if (client_sock > 0)
    {
        const char* tmp_ip = inet_ntoa(client_addr.sin_addr);
        string client_ip = tmp_ip;
        int client_port = ntohs(client_addr.sin_port);
        printf("client %d connect sucessfully, ip : %s port : %d\n", client_sock, client_ip.c_str(), client_port);

        //AddressUnit addr;
        //addr.ip = client_ip;
        //addr.port = client_port;
        //m_memo.insert(pair<AddressUnit, int>(addr, client_sock));
        //printf("client %d added in memo.\n", client_sock);

        std::thread t = std::thread(&ServerSocket::handleMessage, this, client_sock);
        t.detach();
    }
//		else
//		{
//			printf("are you connecting again???\n");
//			sleep(2);
//		}
}

void ServerSocket::run()
{
    while(true)
    {
        listenClients();
    }
}

void ServerSocket::configServer()
{    
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    struct sockaddr_in socket_addr;
    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = inet_addr(m_ip.data());
    socket_addr.sin_port = htons(m_port);

//    int optionVal = 1;
//    if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&optionVal, sizeof(optionVal)) < 0)
//    {
//        printf("setsockopt error!");
//    }

    bind(m_socket, (struct sockaddr*)&socket_addr, sizeof(socket_addr));

}
