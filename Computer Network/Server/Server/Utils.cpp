
#include "Utils.h"
#include "WinSock2.h"
namespace server {
	UDPSocket Utils::CreateUDPSocket()
	{
		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (s != INVALID_SOCKET) {
			server::UDPSocket udp(s);
			return udp;
		}
		
		return NULL;
	}
}