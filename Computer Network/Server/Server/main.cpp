#include "WinSock2.h"
//#include <iostream>
#include <cstdio>
//#include "Utils.h"
//#include "UDPSocket.h"
//#include "SocketAddress.h"
#pragma warning(disable:4996)
//using namespace  server;
int main()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	/*
	uint32_t nadr = inet_addr("192.168.0.104");
	SocketAddress address = SocketAddress(ntohl(nadr), 3301);
	std::shared_ptr<UDPSocket> socket = Utils::CreateUDPSocket();
	socket->Bind(address);
	char buffer[10] = {0,};
	SocketAddress s;
	int t = socket->ReceiveFrom(buffer, 3, s);
	std::cout << buffer;*/
	
	IN_ADDR addr = { 0, };
	char localhost[MAX_PATH];
	gethostname(localhost, MAX_PATH);
	printf("%s\n", localhost);
	hostent* ptr= gethostbyname(localhost);
	while (ptr && ptr->h_name)
	{
		memcpy(&addr, ptr->h_addr_list[0], ptr->h_length);
		printf("%s\n", inet_ntoa(addr));
		
		ptr++;
	}
	//DESKTOP - DRV7QUF
	//192.168.123.2
	WSACleanup();
	return 0;
}