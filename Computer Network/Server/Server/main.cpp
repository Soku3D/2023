#include "WinSock2.h"
#include <iostream>
#include <cstdio>
#include "Utils.h"
#include "UDPSocket.h"
#include "SocketAddress.h"

using namespace  server;
int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	
	uint32_t nadr = inet_addr("192.168.0.104");
	SocketAddress address = SocketAddress(ntohl(nadr), 3301);
	std::shared_ptr<UDPSocket> socket = Utils::CreateUDPSocket();
	socket->Bind(address);
	char buffer[10] = {0,};
	SocketAddress s;
	int t = socket->ReceiveFrom(buffer, 3, s);
	std::cout << buffer;
	
	WSACleanup();
	return 0;
}