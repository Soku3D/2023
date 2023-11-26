#include "SocketAddr.h"
#include <ws2tcpip.h>
namespace sock {
	SocketAddr::SocketAddr()
	{
		GetSockAddrIn()->sin_family = AF_INET;
		GetSockAddrIn()->sin_addr.S_un.S_addr = htonl(0);
		GetSockAddrIn()->sin_port = htons(0);
	}
	SocketAddr::SocketAddr(const sockaddr* addr, uint16_t port)
	{
		this->m_sockAddr = *addr;
		GetSockAddrIn()->sin_family = AF_INET;
		GetSockAddrIn()->sin_port = htons(port);
	}
	SocketAddr::SocketAddr(uint32_t addr, uint16_t port)
	{
		GetSockAddrIn()->sin_family = AF_INET;
		GetSockAddrIn()->sin_addr.S_un.S_addr = htonl(addr);
		GetSockAddrIn()->sin_port = htons(port);
	}

	SocketAddr::SocketAddr(const SocketAddr& socketAddr)
	{
		*this = socketAddr;
	}
	std::string SocketAddr::ToString()
	{
#if _WIN32
		const sockaddr_in* s = SocketAddr::GetSockAddrIn();
		char destinationBuffer[128];
		InetNtop(s->sin_family, const_cast<in_addr*>(&s->sin_addr), destinationBuffer, sizeof(destinationBuffer));
		std::string str = destinationBuffer + ':' + ntohs(s->sin_port);
		printf("ip : %s", str.c_str());

		return str;
#endif
	}
}
