#pragma once
#include <WinSock2.h>
#include <memory>
#include "SocketAddr.h"
namespace sock {
	class UDPSocket {
    public:
        int Bind(SocketAddr& socketAddr);
        int SendTo(const void* inData, int inLen, const SocketAddr& inTo);
        int RecvFrom(void* outData, int outLen, SocketAddr& outFrom);
    private:
        friend class ServerUtils;
        SOCKET m_socket;
        UDPSocket();
    };
    using UDPSocketPtr = std::shared_ptr<UDPSocket>;
}
