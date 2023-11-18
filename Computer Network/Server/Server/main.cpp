#include "WinSock2.h"
#include <iostream>
#include <cstdio>
#include "Utils.h"
#include "UDPSocket.h"
#include "SocketAddress.h"

using namespace  server;
int main()
{
	//WSADATA data;
	//WSAStartup(MAKEWORD(2, 2), &data);
	//IN_ADDR HostAddr = server::Utils::GetHostAddr();
	//server::UDPSocket udp = server::Utils::CreateUDPSocket();
	//SocketAddress s = SocketAddress(ntohl(HostAddr.S_un.S_addr), 80);
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
	server::UDPSocket udp = server::Utils::CreateUDPSocket();
	char localhostname[MAX_PATH];
	if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR)
	{
		return -1;
	}
	IN_ADDR addr = { 0, };
	HOSTENT* entPtr = gethostbyname(localhostname);
	while (entPtr->h_name && entPtr)
	{
		if (entPtr->h_addrtype == PF_INET)
		{
			memcpy(&addr, entPtr->h_addr_list[0], entPtr->h_length);
		}
		entPtr++;
	}
	uint32_t naddr = inet_addr("192.168.0.104");
	SocketAddress s = SocketAddress(ntohl(naddr), 1180);
	udp.Bind(s);
	SocketAddress client;
	char buffer[100];
	udp.ReceiveFrom(buffer, 99, client);

	WSACleanup();
	return 0;
}