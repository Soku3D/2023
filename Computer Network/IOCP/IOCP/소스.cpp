#include <WinSock2.h>
#include <WS2tcpip.h>
#include <memory>
#include <vector>
#include <string>
#pragma warning(disable:4996)
using TCPPTR = std::shared_ptr<SOCKET>;
void GetAddr(const std::string& port, sockaddr& addr);
void InitServer();
void EndServer();
fd_set* FillSetFromVector(fd_set& outSet, const std::vector<TCPPTR>* readIn);
void FillVectorFromSet(const fd_set& remainRead, const std::vector<TCPPTR>* readIn, std::vector<TCPPTR>* readOut);
int Select(const std::vector<TCPPTR>* readIn,
	std::vector<TCPPTR>* readOut);
void DoTcpLoop();
int main()
{
	InitServer();
	DoTcpLoop();
	EndServer();
}

void GetAddr(const std::string& port, sockaddr& addr)
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

void InitServer()
{
	WSAData data;
	WSAStartup(MAKEWORD(2, 2), &data);	
}

void EndServer()
{
	WSACleanup();
}

fd_set* FillSetFromVector(fd_set& outSet, const std::vector<TCPPTR>* readIn)
{
	if (readIn)
	{
		FD_ZERO(&outSet);
		for (const TCPPTR& socket : *readIn)
		{
			FD_SET(*socket, &outSet);
		}
		return &outSet;
	}
	return nullptr;
}

void FillVectorFromSet(const fd_set& remainRead, const std::vector<TCPPTR>* readIn, std::vector<TCPPTR>* readOut)
{
	if (readIn && readOut)
	{
		readOut->clear();
		for (const TCPPTR& socket : *readIn)
		{
			if (FD_ISSET(*socket, &remainRead))
			{
				readOut->push_back(socket);
			}
		}
	}
}

int Select(const std::vector<TCPPTR>* readIn, std::vector<TCPPTR>* readOut)
{
	fd_set read;
	fd_set* readPtr = FillSetFromVector(read, readIn);
	int count = select(0, readPtr, nullptr, nullptr, nullptr);

	if (count > 0)
	{
		FillVectorFromSet(read, readIn, readOut);
	}
	return count;
}

void DoTcpLoop()
{
	InitServer();
	TCPPTR listenSocket = std::make_shared<SOCKET>(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
	sockaddr serverAddr = { 0, };
	GetAddr("7777", serverAddr);
	
	
	int ret = bind(*listenSocket, &serverAddr, sizeof(serverAddr));
	printf("%d\n", ret);
	ret = listen(*listenSocket, 32);
	printf("%d\n", ret);

	printf("server opend!\n");

	std::vector<TCPPTR> readBlockSockets;
	std::vector<TCPPTR> readableSockets;
	readBlockSockets.push_back(listenSocket);

	while (true)
	{
		if (Select(&readBlockSockets, &readableSockets) > 0)
		{
			for (const TCPPTR& socket : readableSockets)
			{
				if (socket == listenSocket)
				{
					sockaddr clientAddr;
					int len = sizeof(clientAddr);
					TCPPTR clientSocket = std::make_shared<SOCKET>(accept(*socket, &clientAddr, &len));
					readBlockSockets.push_back(clientSocket);
					printf("connected!\n");
				}
				else
				{
					char buff[7] = "";
					int size = recv(*socket, buff, sizeof(buff), 0);
					if (size > 0)
						printf("client : %s\n", buff);
				}
			}
		}
		else
		{
			printf("gameLoop\n");
		}

		
	}
	for (TCPPTR& socket : readableSockets)
	{
		closesocket(*socket);
	}
	for (TCPPTR& socket : readBlockSockets)
	{
		closesocket(*socket);
	}
	EndServer();
}
