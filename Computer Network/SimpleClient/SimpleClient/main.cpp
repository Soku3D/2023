#include <Winsock2.h>
#include <cstdio>
#include <WS2tcpip.h>
#include <string>
#include <iostream>
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)
void getAddr(std::string port , sockaddr& addr)
{
	char localhostname[MAX_PATH];
	gethostname(localhostname, MAX_PATH);
	std::string host = localhostname;
	
	addrinfo hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;

	addrinfo* result = nullptr;
	getaddrinfo(host.c_str(), port.c_str(), &hint, &result);

	addr = *(result->ai_addr);
	freeaddrinfo(result);
}
void GetServerAddr(std::string port, sockaddr& addr)
{
	
	std::string host = "58.224.152.152";

	addrinfo hint;
	ZeroMemory(&hint, sizeof(hint));
	hint.ai_family = AF_INET;
	
	addrinfo* result = nullptr;
	getaddrinfo(host.c_str(), port.c_str(), &hint, &result);

	addr = *(result->ai_addr);


	freeaddrinfo(result);
	

}
int main()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);

	SOCKET tcpClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	u_long arg = 1;
	//ioctlsocket(tcpClient, FIONBIO, &arg);

	
	sockaddr m_addr;
	sockaddr serverAddr;
	getAddr("3333", m_addr);
	GetServerAddr("7777", serverAddr);
	//getAddr("7778", serverAddr);
	bind(tcpClient, &m_addr, sizeof(m_addr));
	sockaddr_in* tt = (sockaddr_in*)&serverAddr;
	printf("%s:%d\n", inet_ntoa(tt->sin_addr), ntohs(tt->sin_port));

	connect(tcpClient, &serverAddr, sizeof(serverAddr));

	while (1)
	{
		std::string str;
		std::cin >> str;
		std::cout << str;
		send(tcpClient, str.c_str(), str.size(),0);
	}
	
	closesocket(tcpClient);
	WSAGetLastError();
	WSACleanup();
}