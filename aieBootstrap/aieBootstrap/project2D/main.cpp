#include "Application2D.h"
#include "Defines.h"

int main() 
{
	Application2D* app = new Application2D();
	app->run("AIE", GRID_SIZE * NODE_SIZE, GRID_SIZE * NODE_SIZE, false);
	delete app;

	return 0;
}