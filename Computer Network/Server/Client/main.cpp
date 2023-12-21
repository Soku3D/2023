
#include <iostream>
#include <cstdio>
#include "Utils.h"
#include "UDPSocket.h"
#include "SocketAddress.h"
using namespace server;
using UDPSocketPtr = std::shared_ptr<UDPSocket>;
int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	uint32_t nadr = inet_addr("192.168.0.104");
	SocketAddress s = SocketAddress(ntohl(nadr), 883);
	SocketAddress serverAddr = SocketAddress(ntohl(nadr), 3301);
	std::shared_ptr<UDPSocket> socket = Utils::CreateUDPSocket();

	socket->Bind(s);
	SocketAddress clientAddr;
	char buffer[100] = "123";
	int t = socket->SendTo(buffer, 100, serverAddr);
	std::cout << t;


	WSACleanup();
}