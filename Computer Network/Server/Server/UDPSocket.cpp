#include "UDPSocket.h"

namespace server {
	UDPSocket::~UDPSocket()
	{
		closesocket(m_socket);
	}
	int  UDPSocket::Bind(const SocketAddress& inToAddress)
	{
		int err = bind(m_socket, &inToAddress.m_sockAddr, inToAddress.GetSize());
		
		if (err == 0)
		{
			return NO_ERROR;
		}
		return -1;
	}
	int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo)
	{
		sendto(m_socket, reinterpret_cast<const char*>(inData),
			inLen, 0, &inTo.m_sockAddr, inTo.GetSize());
		return 1;
	}
	int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom)
	{
		int fromLength = 0;
		int recvBytes = recvfrom(m_socket, reinterpret_cast<char*>(inBuffer),
			inLen, 0, &outFrom.m_sockAddr,&fromLength);
		if (recvBytes > -1)
		{
			return recvBytes;
		}
		//SocketUtils::ReportError
		return -1;
	}
}