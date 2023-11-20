#pragma once
#include <iostream>
#include <WinSock2.h>


namespace server {
	class SocketAddress {
	public:
		friend class UDPSocket;
		SocketAddress(uint32_t inAddress, uint16_t inPort);
		SocketAddress(const sockaddr& inSockAddr);
		SocketAddress()
		{
			GetAsSockAddrIn()->sin_family = AF_INET;
			GetAsSockAddrIn()->sin_port = 0;
		}
		inline size_t GetSize() const { return sizeof(m_sockAddr); }
	private:
		sockaddr m_sockAddr;
		sockaddr_in* GetAsSockAddrIn()
		{
			return reinterpret_cast<sockaddr_in*>(&m_sockAddr);
		}
	};
}