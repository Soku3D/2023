#include "RenderApp.h"
#include "UDPServer.h"

int main()
{
	soku::RenderApp app;
	sock::UDPServer server(30);
	if (!server.Initialize(3001))
	{
		std::cout << "initserver Failed\n";
	}
	server.Bind();
	printf("str : %s\n", server.outBuffer);
	sock::SocketAddr client;
	server.RecvFrom(client);
	printf("str : %s\n", server.outBuffer);

	WSACleanup();
	if (app.Initialize())
	{
		return app.Run();
	}
	return -1;
}