#include <WinSock2.h>
#include <cstdio>
#pragma comment(lib,"ws2_32")
#pragma warning(disable:4996)
#define PORT_NUM    7777
#define BLOG_SIZE   5
#define MAX_MSG_LEN 256
#define warning
SOCKET SetTCPServer(short pnum, int blog);
void AcceptLoop(SOCKET sock);
IN_ADDR GetDefaultMyIP();

int main() {
	WSADATA wsaData;
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (err != 0) {
		printf("WSAStartup Error!");
		return 1;
	}

	SOCKET socket = SetTCPServer(PORT_NUM, BLOG_SIZE);
	AcceptLoop(socket);
	
	closesocket(socket);
	
	WSACleanup();
}
SOCKET SetTCPServer(short pnum, int blog)
{
	SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN serverAddr = { 0 };
	serverAddr.sin_addr = GetDefaultMyIP();
	serverAddr.sin_family = PF_INET;
	serverAddr.sin_port = htons(pnum);
	
	printf("ip = %s",inet_ntoa(serverAddr.sin_addr));
	int err = bind(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (err == SOCKET_ERROR)
		return -1;
	err = listen(sock, blog);
	if (err == SOCKET_ERROR)
		return -1;

	return sock;
}
void AcceptLoop(SOCKET sock)
{
	SOCKET dosock;
	SOCKADDR_IN cliaddr = { 0 };
	int len = sizeof(cliaddr);
	while (true)
	{
		dosock = accept(sock, (SOCKADDR*)&cliaddr, &len);
		if (dosock == SOCKET_ERROR) {
			perror("accept error ocured");
			break;
		}
		printf("CLIENT = %s:%d", inet_ntoa(cliaddr.sin_addr), ntohs(cliaddr.sin_port));
	}
}
IN_ADDR GetDefaultMyIP()
{
	IN_ADDR addr = { 0 };
	char name[MAX_PATH];
	int err = gethostname(name, MAX_PATH);

	if (err == SOCKET_ERROR) 
	{
		printf("gethostname error");
		return addr;
	}
	hostent* ent = gethostbyname(name);
	while (ent && ent->h_name)
	{
		if (ent->h_addrtype == PF_INET)
		{
			memcpy(&addr, ent->h_addr_list[0], ent->h_length);
			break;
		}
		ent++;
	}
	return addr;
}
