#include "UDPServer.h"

namespace sock {
	UDPServer::UDPServer(int buffSize)
		:bufferSize(buffSize)
	{
		inBuffer = new char[buffSize];
		outBuffer = new char[buffSize];
	}
	bool UDPServer::Initialize(uint16_t port)
	{
		if (!ServerUtils::StaticInit())
			return false;
		char localhost[MAX_PATH];
		IN_ADDR address = { 0, };

		if (gethostname(localhost, MAX_PATH) == SOCKET_ERROR)
		{
			return false;
		}
		char service[] = "0";
		addrinfo* ret = nullptr;
		addrinfo hint;
		ZeroMemory(&hint, sizeof(hint));
		hint.ai_family = AF_INET;
		getaddrinfo(localhost, service, &hint, &ret);
		
		SocketAddr sockAddr(ret->ai_addr, port);
		sockAddr.ToString();
		m_serverAddr = sockAddr;
		m_udpSockPtr = ServerUtils::CreateUDPSocket();
		if (m_udpSockPtr == nullptr)
			return false;

		return true;
	}
	bool UDPServer::Bind()
	{
		m_udpSockPtr->Bind(m_serverAddr);

		return true;
	}
	bool sock::UDPServer::RecvFrom(SocketAddr& outfrom)
	{
		m_udpSockPtr->RecvFrom(outBuffer, bufferSize, outfrom);
		m_clientAddr = outfrom;
		return true;
	}
	bool UDPServer::SendTo(const SocketAddr& inTo)
	{
		m_udpSockPtr->SendTo(inBuffer, bufferSize, inTo);
		return true;
	}
}

