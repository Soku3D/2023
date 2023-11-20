#include "RenderApp.h"
#include "UDPSocket.h"

#pragma comment(lib, "ServerLib.lib")
int main()
{
	soku::RenderApp app;

	std::cout<< ServerSock::UDPSocket::Abs(1, 2);
	if (app.Initialize())
	{
		return app.Run();
	}
	return -1;
}