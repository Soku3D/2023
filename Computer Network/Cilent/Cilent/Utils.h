#pragma once
#pragma warning(disable:4996)
#include "WinSock2.h"
#include "UDPSocket.h"
#include <memory>

namespace server {
	class UDPSocket;
	class Utils {
	public:
		static IN_ADDR GetHostAddr()
		{
			char localhost[MAX_PATH];
			IN_ADDR address = { 0, };

			if (gethostname(localhost, MAX_PATH) == SOCKET_ERROR)
			{
				return address;
			}
			HOSTENT* ent = gethostbyname(localhost);
			while (ent && ent->h_name)
			{
				if (ent->h_addrtype == PF_INET)
				{
					memcpy(&address, ent->h_addr_list[0], ent->h_length);
					break;
				}
				ent++;
			}
			return address;
			
		}
		static std::shared_ptr<UDPSocket> CreateUDPSocket();
	};
}