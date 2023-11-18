#include "SocketAddress.h"

namespace server {
	SocketAddress::SocketAddress(uint32_t inaddr, uint16_t inport) {
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inaddr);
		GetAsSockAddrIn()->sin_port = htons(inport);
	}
	SocketAddress::SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&m_sockAddr, &inSockAddr, sizeof(sockaddr));
	}
}