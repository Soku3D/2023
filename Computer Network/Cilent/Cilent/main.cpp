#include "WinSock2.h"
#include <iostream>
#include <WS2tcpip.h>
sockaddr* getaddr(const std::string& server);
//using namespace  server;
int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
	// Initialize socket & bind it
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr* m_addr = getaddr("1001");
	bind(s, m_addr, sizeof(sockaddr));
	
	sockaddr* serveraddr = getaddr("3001");
	int err = connect(s, serveraddr, sizeof(sockaddr));
	if (err > 0)
		printf("no err\n");

	// recv from client
	char buffer[] = "stptptpt";
	send(s, buffer, sizeof(buffer), 0);
	//printf("%s", buffer);

	WSACleanup();
}

sockaddr* getaddr(const std::string& server)
{
	addrinfo* result = nullptr;
	addrinfo hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	char hostname[MAX_PATH];
	gethostname(hostname, MAX_PATH);
	std::string host = hostname;
	getaddrinfo(host.c_str(), server.c_str(), &hint, &result);
	sockaddr* m_addr = result->ai_addr;
	freeaddrinfo(result);
	return m_addr;
}
