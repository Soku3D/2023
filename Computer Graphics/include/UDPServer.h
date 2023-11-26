#include "SocketAddr.h"
#include "UDPSocket.h"
#include "ServerUtils.h"
#include "ws2tcpip.h"
namespace sock{
	class UDPServer {
	public:
		UDPServer(int buffSize);
		bool Initialize(uint16_t port);
		bool Bind();
		bool SendTo(const SocketAddr& inTo);
		bool RecvFrom(SocketAddr& outfrom);

		char* inBuffer;
		char* outBuffer;
		int bufferSize;
	private:
		UDPSocketPtr m_udpSockPtr;
		SocketAddr m_serverAddr;
		SocketAddr m_clientAddr;

	};
}
