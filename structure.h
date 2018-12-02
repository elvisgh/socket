#include <iostream>
#include <string>
#include <string.h>

using namespace std;

struct MessageBody
{
	char destIp[16];
	int destPort;
	char message[40];
};

struct AddressUnit
{
    string ip;
    int port;
};

bool operator<(const AddressUnit& left, const AddressUnit& right)
{
    return left.ip < right.ip;
}
