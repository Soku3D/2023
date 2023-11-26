#include "UDPSocket.h"
namespace sock {
	int UDPSocket::Bind(SocketAddr& socketAddr)
	{
		int ret = bind(m_socket, &socketAddr.m_sockAddr, socketAddr.GetSize());
		return ret;
	}
	int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddr& inTo)
	{
		int sentByte = sendto(m_socket, reinterpret_cast<const char*>(inData), inLen, 0,
			&inTo.m_sockAddr, inTo.GetSize());
		return sentByte;
	}
	int UDPSocket::RecvFrom(void* inBuffer, int outLen, SocketAddr& outFrom)
	{
		int fromLen = outFrom.GetSize();
		int readByte = recvfrom(m_socket, reinterpret_cast<char*>(inBuffer), outLen, 0,
			&outFrom.m_sockAddr, &fromLen);
		return readByte;
	}
	UDPSocket::UDPSocket()
	{
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
}

