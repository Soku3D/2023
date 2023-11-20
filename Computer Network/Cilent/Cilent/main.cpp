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
	SocketAddress s = SocketAddress(ntohl(nadr), 883);
	SocketAddress serverAddr = SocketAddress(ntohl(nadr), 3301);

	std::shared_ptr<UDPSocket> udpPtr = Utils::CreateUDPSocket();
	udpPtr->Bind(s);
	SocketAddress clientAddr;
	char buffer[100] = "123";
	int t = udpPtr->SendTo(buffer, 100, serverAddr);
	std::cout << t;


	WSACleanup();
}