#include "../Window/Window.h"
#include "../Utils/Camera.h"
#include <cstdlib>
#include <ctime>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main(int argc, char* args[])
{
	srand(static_cast <unsigned> (time(0)));

	Camera* camera = new Camera(glm::vec3(0,0,0),glm::vec3(0,0,0));

	Window GameWindow("Cristian Toporascu - Ray Tracing", SCREEN_WIDTH, SCREEN_HEIGHT);
	GameWindow.initSDL(camera);

	system("pause");
	return 0;
}
