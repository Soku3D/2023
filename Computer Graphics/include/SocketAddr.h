#pragma once
#include <WinSock2.h>
#include <iostream>
#include <string>

namespace sock {
	class SocketAddr {
	public:
		SocketAddr();
		SocketAddr(const sockaddr* addr, uint16_t port);
		SocketAddr(uint32_t addr, uint16_t port);
		explicit SocketAddr(const SocketAddr& socketAddr);
		int GetSize() const { return sizeof(m_sockAddr); }
		std::string	ToString();
	private:
		friend class UDPSocket;
		sockaddr m_sockAddr;
		sockaddr_in* GetSockAddrIn()
		{
			return reinterpret_cast<sockaddr_in*>(&m_sockAddr);
		}
	};
}