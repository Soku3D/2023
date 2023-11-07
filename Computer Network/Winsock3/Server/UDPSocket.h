#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

namespace sock {
	class UDPSocket {
	public:
		~UDPSocket();

	private:
		friend class SocketUtil;
	};
}