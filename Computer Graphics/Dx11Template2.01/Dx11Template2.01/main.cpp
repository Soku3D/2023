#include "RenderApp.h"

int main()
{
	soku::RenderApp app;

	if (app.Initialize())
	{
		return app.Run();
	}
	return -1;
}