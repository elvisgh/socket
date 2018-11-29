#include <iostream>
#include <string.h>

using namespace std;

struct MessageBody
{
	char destIp[16];
	int destPort;
	char message[40];
};
