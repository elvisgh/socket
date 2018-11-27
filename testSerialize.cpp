#include <iostream>
#include <string>

using namespace std;

struct MessageBody
{
    string ip;
    int port;
};

int main()
{
    MessageBody mb;
    mb.ip = "127.0.0.1";
    mb.port = 12333;

    char *buffer[1024];
