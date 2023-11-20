
#include "Utils.h"
#include "WinSock2.h"
namespace server {
	std::shared_ptr<UDPSocket> Utils::CreateUDPSocket()
	{
		SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (s != INVALID_SOCKET) {
			std::shared_ptr<server::UDPSocket> udp(new UDPSocket(s));
			return udp;
		}
		
		return NULL;
	}
}