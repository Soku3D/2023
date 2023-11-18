#pragma once
#pragma warning(disable:4996)
#include "WinSock2.h"
#include "UDPSocket.h"


namespace server {
	class UDPSocket;
	class Utils {
	public:
		static IN_ADDR GetHostAddr()
		{
			IN_ADDR addr = { 0, };
			char localhostname[MAX_PATH];
			if (gethostname(localhostname, MAX_PATH) == SOCKET_ERROR)
			{
				return addr;
			}
			HOSTENT* entPtr = gethostbyname(localhostname);
			while (entPtr->h_name && entPtr)
			{
				if (entPtr->h_addrtype == PF_INET)
				{
					memcpy(&addr, entPtr->h_addr_list[0], entPtr->h_length);
				}
				entPtr++;
			}
			return addr;
			
		}
		static UDPSocket CreateUDPSocket();
	};
}