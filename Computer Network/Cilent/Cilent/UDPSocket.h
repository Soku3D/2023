#pragma once
#include <WinSock2.h>
#include "SocketAddress.h"
namespace server {
	class UDPSocket {
	public:
		~UDPSocket();
		int Bind(const SocketAddress& inToAddress);
		int SendTo(const void* inData, int inLen, const SocketAddress& inTo);
		int ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom);
	private:
		friend class Utils;
		UDPSocket(SOCKET inSocket) : m_socket(inSocket) {}
		SOCKET m_socket;
	};

}