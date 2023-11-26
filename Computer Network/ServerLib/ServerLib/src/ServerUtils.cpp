#include "ServerUtils.h"

namespace sock {
	UDPSocketPtr ServerUtils::CreateUDPSocket()
	{
		return UDPSocketPtr(new UDPSocket());
	}
	bool ServerUtils::StaticInit()
	{
#if _WIN32
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR)
		{
			return false;
		}
#endif
		return true;
	}

	void ServerUtils::CleanUp()
	{
#if _WIN32
		WSACleanup();
#endif
	}

}
