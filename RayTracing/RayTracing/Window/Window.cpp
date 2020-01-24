#include "Window.h"
#include "../Utils/Clock.h"
#include "../Primitives/Sphere.h"
#include "../Primitives/Plane.h"
#include "../Primitives/Triangle.h"
#include "../Utils/Light.h"
#include "../Base/Mesh.h"
#include "../Primitives/Object.h"
#include <thread>
#include <chrono>
#include <iostream>

using Time = std::chrono::high_resolution_clock;
using fsec = std::chrono::duration<float>;

std::vector<std::vector<Object_Base*>> sceneList;

Light light(glm::vec3(-7, 50, 3), glm::vec3(0.5f, 0.5f, 0.5f), Colour(glm::vec3(1.0f, 1.0f, 1.0f)), 50, 9);

Window::Window(std::string title, int width, int height, int frameRate)
{
	this->m_title = title;
	this->m_width = width;
	this->m_height = height;
	this->m_frameRate = frameRate;
}

Window::~Window() {
	if (this->m_renderSurface) {
		delete this->m_renderSurface;
		delete this->m_camera;
	}
}

void Window::InitSDL_API() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Unable to initialize SDL: %s", SDL_GetError());
		return;
	}
	this->SDLWindowRunning = true;
}

void Window::DestroySDL_API() {
	SDL_Log("Closing window '%s'", this->m_title.c_str());
	SDL_DestroyWindow(this->m_window);
	SDL_FreeSurface(this->m_windowSurface);
	SDL_Quit();
	return;
}

void Window::initSDL(Camera* camera)
{
	this->InitSDL_API();

	this->m_window = SDL_CreateWindow(
		this->m_title.c_str(),
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		this->m_width,
		this->m_height,
		SDL_WINDOW_SHOWN /* | SDL_WINDOW_RESIZABLE */
	);


	if (this->m_window == nullptr)
	{
		SDL_LogError(SDL_LOG_CATEGORY_RENDER, "Failed to initialize window: '%s'", SDL_GetError());
		return;
	}

	this->m_windowSurface = SDL_GetWindowSurface(this->m_window);
	this->m_renderSurface = new RenderSurface(this->m_width, this->m_height, this->m_windowSurface);

	SDL_Log("Window has been initialized: '%s'", this->m_title.c_str());

	this->screenData = new int* [this->m_width];
	for (int x = 0; x < this->m_width; x++) {
		this->screenData[x] = new int[this->m_height];
		for (int y = 0; y < this->m_height; y++) {
			this->screenData[x][y] = -1;
		}
	}

	this->m_camera = camera;
	this->generateScreen();
}

void Window::generateScreen() {
	SDL_Log("-------------------------------------");
	SDL_Log("\tControls:");
	SDL_Log("\t\tWS: \tForward/Back");
	SDL_Log("\t\tAD: \tLeft/Right");
	SDL_Log("\t\tSpace: \tUp");
	SDL_Log("\t\tLCtrl: \tDown");
	SDL_Log("-------------------------------------");
	SDL_Log("\tDetails:");
	SDL_Log("\t\t1: \tNo shadows(Default)");
	SDL_Log("\t\t2: \tHard shadows");
	SDL_Log("\t\t2: \tArea light/Soft shadows");
	SDL_Log("\t\t4: \tReflectivness and refraction");
	SDL_Log("\t\t6: \tRandomise shadow sampling");
	SDL_Log("-------------------------------------");
	SDL_Log("\tLight:");
	SDL_Log("\t\tArrows: \tMove");
	SDL_Log("-------------------------------------");
	SDL_Log("\tSoft-shadow sample number:");
	SDL_Log("\t\t7: \t9");
	SDL_Log("\t\t8: \t16");
	SDL_Log("\t\t9: \t32");
	SDL_Log("\t\t0: \t64");
	SDL_Log("-------------------------------------");
	SDL_Log("\tLoad scene:");
	SDL_Log("\t\tL: \tDefault scene");
	SDL_Log("\t\tK: \tReflective spheres scene");
	SDL_Log("-------------------------------------");

	Clock gameWholeClock;
	const int sleepTimePerFR = (1000.0f / (float)this->m_frameRate);

	Clock deltaTimeClock;
	float deltaTime = 0;

	SDL_Event currentEvent;
	std::vector<Object_Base*> sceneBuilder;

#pragma region Scene1
	Mesh* firstObjectTest = Mesh::importObjFile("SimpleObjs/teapot_simple_smooth.obj", glm::vec3(-2, 0, -10), Colour(0.75f, 0.15f, 0.45f), 45, true, false);
	Object* object = new Object(firstObjectTest);
	sceneBuilder.push_back(object);

	Mesh* secondObjectTest = Mesh::importObjFile("SimpleObjs/conan.obj", glm::vec3(3, -4, -17), Colour(0.15f, 0.15f, 0.45f), 300, true);
	Object* object2 = new Object(secondObjectTest);
	sceneBuilder.push_back(object2);

	sceneBuilder.push_back(new Plane(glm::vec3(0, -4, -30), glm::vec3(0, 1, 0), Colour(0.0f, 0.44f, 0.23f), true));
	sceneBuilder.push_back(new Sphere(4, glm::vec3(-4, 0.1, -20), Colour(0.2f, 0.32f, 0.36f), true));
	sceneBuilder.push_back(new Sphere(3, glm::vec3(5, 0.1, -10), Colour(0.90f, 0.22f, 0.11f), true));

	sceneList.push_back(sceneBuilder);
	sceneBuilder.clear();
#pragma endregion

#pragma region SceneSpheres
	sceneBuilder.push_back(new Plane(glm::vec3(0, -4, -30), glm::vec3(0, 1, 0), Colour(0.0f, 0.44f, 0.23f)));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(8, 0.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(4, 0.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(0, 0.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-4, 0.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-8, 0.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(8, 0.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(4, 0.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(0, 0.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-4, 0.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-8, 0.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(8, 0.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(4, 0.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(0, 0.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-4, 0.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-8, 0.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
									  
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(8, 4.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(4, 4.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(0, 4.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-4, 4.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-8, 4.1, -10), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(8, 4.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(4, 4.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(0, 4.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-4, 4.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-8, 4.1, -15), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(8, 4.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(4, 4.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(0, 4.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-4, 4.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneBuilder.push_back(new Sphere(1.5f, glm::vec3(-8, 4.1, -20), Colour(0.90f, 0.22f, 0.11f), true));
	sceneList.push_back(sceneBuilder);
	sceneBuilder.clear();
#pragma endregion

	while (this->SDLWindowRunning)
	{
		gameWholeClock.Start();

		deltaTime = (float)deltaTimeClock.getTimeMilliseconds() / 1000.0f;
		deltaTimeClock.Start();

		if (SDL_PollEvent(&currentEvent)) {
			this->executeActionSDL(currentEvent);
		}

#pragma region Keybindings
		if (!this->m_renderSurface->getRenderingState()) {
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_ESCAPE)) {
				this->SDLWindowRunning = false;
				SDL_Log("SDL has been destroyed, closing action fired.");
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_L)) {
				if (this->sceneNumber == 1) {
					SDL_Log("The default scene will be loaded.");
					this->hardShadowsOn = false;
					this->reflectivenessOn = false;
					this->areaLight = false;
					this->randShadows = false;
				}
				this->sceneNumber = 0;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_K)) {
				if (this->sceneNumber == 0) {
					SDL_Log("The reflective sphere scene will be loaded.");
					this->hardShadowsOn = false;
					this->reflectivenessOn = false;
					this->areaLight = false;
					this->randShadows = false;
				}
				this->sceneNumber = 1;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_W))
				this->m_camera->setPos(this->m_camera->getPos() + this->m_camera->getForwardVector() * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_S))
				this->m_camera->setPos(this->m_camera->getPos() - this->m_camera->getForwardVector() * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_A))
				this->m_camera->setPos(this->m_camera->getPos() + this->m_camera->getRightVector() * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_D))
				this->m_camera->setPos(this->m_camera->getPos() - this->m_camera->getRightVector() * deltaTime);

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_SPACE))
				this->m_camera->setPos(this->m_camera->getPos() + glm::vec3(0, -1, 0) * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_LCTRL))
				this->m_camera->setPos(this->m_camera->getPos() + glm::vec3(0, 1, 0) * deltaTime);

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_UP))
				light.setPos(light.getPos() + glm::vec3(0, 0, -10.0f) * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_DOWN))
				light.setPos(light.getPos() + glm::vec3(0, 0, 10.0f) * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_LEFT))
				light.setPos(light.getPos() + glm::vec3(-10.0f, 0, 0) * deltaTime);
			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_RIGHT))
				light.setPos(light.getPos() + glm::vec3(10.0f, 0, 0) * deltaTime);

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_1)) {
				if (hardShadowsOn || reflectivenessOn || areaLight || randShadows) {
					SDL_Log("\tAll details will be nulified in the scene");
				}
				this->hardShadowsOn = false;
				this->reflectivenessOn = false;
				this->areaLight = false;
				this->randShadows = false;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_2)) {
				if (!hardShadowsOn) {
					SDL_Log("\tHard shadows will be calculated to the objects");
				}
				this->hardShadowsOn = true;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_3)) {
				if (!areaLight) {
					SDL_Log("\tSoft shadows will be calculated to the objects");
				}
				this->areaLight = true;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_4)) {
				if (!reflectivenessOn) {
					SDL_Log("\tReflection and refraction will be calculated to the objects");
				}
				this->reflectivenessOn = true;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_6)) {
				if (!randShadows) {
					SDL_Log("\tSoft shadows will be calculated to the objects in a randomised manner");
				}
				this->randShadows = true;
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_7)) {
				if (light.getSamplesNumber() != 9) {
					SDL_Log("\tNumber of samples will be set to 9");
					light.setSamplesNumber(9);
				}
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_8)) {
				if (light.getSamplesNumber() != 16) {
					SDL_Log("\tNumber of samples will be set to 16");
					light.setSamplesNumber(16);
				}
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_9)) {
				if (light.getSamplesNumber() != 32) {
					SDL_Log("\tNumber of samples will be set to 32");
					light.setSamplesNumber(32);
				}
			}

			if (this->m_keyboard.isKeyDown(KeyCodes::K_CODE_0)) {
				if (light.getSamplesNumber() != 64) {
					SDL_Log("\tNumber of samples will be set to 64");
					light.setSamplesNumber(64);
				}
			}
		}
#pragma endregion

		std::chrono::steady_clock::time_point start;
		if (!this->screenUpdated) {
			start = Time::now();
		}

		if (this->m_renderSurface->getRenderingState()) {
			std::vector<std::thread> multiThreading;

			for (int threadNumber = 0; threadNumber < 8; threadNumber++) {
				multiThreading.push_back(std::thread{ &Window::ThreadDrawer, this, (threadNumber * this->m_width) / 8, this->m_width / 8, this->m_renderSurface->getRenderingState() });
			}

			for (int threadNumberCount = 0; threadNumberCount < multiThreading.size(); threadNumberCount++) {
				multiThreading[threadNumberCount].join();
			}
		}
		else {
			ThreadDrawer(0, this->m_width, this->m_renderSurface->getRenderingState());
		}

		if (this->screenUpdated) {
			for (int x = 0; x < this->m_width; x++) {
				for (int y = 0; y < this->m_height; y++) {
					this->m_renderSurface->setPixelSDLMap(x, y, this->screenData[x][y]);

					if (x == this->m_width - 1 && y == this->m_height - 1) {
						SDL_BlitSurface(this->m_renderSurface->getSDLRenderSurface(), nullptr, this->m_windowSurface, nullptr);
						SDL_UpdateWindowSurface(m_window);
					}
				}
			}

			if (this->frameAction) {
				auto end = Time::now();
				fsec duration = end - start;

				std::cout << "DRAWING FRAME: " << duration.count() << std::endl;
				this->frameAction = false;
			}
		}

		const int sleepTime = sleepTimePerFR - gameWholeClock.getTimeMilliseconds();

		if (sleepTime > 0)
			SDL_Delay(sleepTime);
	}

	this->DestroySDL_API();

	for (int i = 0; i < sceneList.size(); i++) {
		for (int j = 0; j < sceneList[i].size(); j++) {
			delete sceneList[i][j];
		}
	}

	return;
}

void Window::ThreadDrawer(int threadStartPoint, int threadWidth, bool check) {
	int step = (!check ? 20 : 1);
	int wavesStep = 100;
	int wavesWidth = (int)(threadWidth / wavesStep);

	if (!this->screenUpdated && check) {
		if (threadStartPoint == 0) {
			this->m_renderSurface->Clear(Colour(168, 251, 255, 255));
		}
		for (unsigned int w = 0; w < wavesStep; w++) {
			for (unsigned int y = 0; y < this->m_height; y += step)
			{
				double aR = this->m_width / this->m_height;
				double nY = (y + 0.5) / this->m_height;
				double rMapY = 1 - 2 * nY;
				double camY = rMapY * tan(glm::radians<float>(this->m_camera->getFOV()) / 2);

				for (unsigned int x = wavesWidth * w + threadStartPoint; x < wavesWidth * (w + 1) + threadStartPoint; x += step)
				{
					/* Clear screen data before redrawing it */
					this->screenData[x][y] = -1;

					double nX = (x + 0.5) / this->m_width;
					double rMapX = (2 * nX - 1) * aR;
					double camX = rMapX * tan(glm::radians<float>(this->m_camera->getFOV()) / 2);

					glm::vec3 camSpace(camX, camY, -1);

					Ray ray(this->m_camera->getPos(), glm::normalize(camSpace - this->m_camera->getPos()));
					HitInfo hit;

					for (unsigned int i = 0; i < sceneList[this->sceneNumber].size(); i++) {
						sceneList[this->sceneNumber][i]->Intersection(ray, hit);
					}

					if (hit.hitObject != nullptr) {
						Colour finalColour = light.CalculateLighting(
							sceneList[this->sceneNumber],
							ray,
							hit,
							128,
							Colour(glm::vec3(0.2f, 0.2f, 0.2f)),
							Colour(glm::vec3(1.0f, 1.0f, 1.0f)),
							Colour(glm::vec3(1.0f, 1.0f, 1.0f)),
							this->hardShadowsOn,
							this->randShadows,
							this->reflectivenessOn,
							this->areaLight);

						if (!check) {
							this->m_renderSurface->setRect(x, y, step, step, finalColour, this->screenData);
						}
						else {
							this->m_renderSurface->setPixel(x, y, finalColour, this->screenData);
						}
					}
				}
			}

			/**
			 * If drawn in waves the update needs to take place here 
			 */

			/*SDL_BlitSurface(this->m_renderSurface->getSDLRenderSurface(), nullptr, this->m_windowSurface, nullptr);
			SDL_UpdateWindowSurface(m_window);*/
		}

		/**
		 * If drawn in waves the update needs to be commented
		 */

		SDL_BlitSurface(this->m_renderSurface->getSDLRenderSurface(), nullptr, this->m_windowSurface, nullptr); // -> Comment if waves
		SDL_UpdateWindowSurface(m_window); // -> Comment if waves

		this->screenUpdated = true;
	}
	else if (!check) {
		if (threadStartPoint == 0) {
			this->m_renderSurface->Clear(Colour(168, 251, 255, 255));
		}
		for (unsigned int y = 0; y < this->m_height; y += step)
		{
			double aR = this->m_width / this->m_height;
			double nY = (y + 0.5) / this->m_height;
			double rMapY = 1 - 2 * nY;
			double camY = rMapY * tan(glm::radians<float>(this->m_camera->getFOV()) / 2);

			for (unsigned int x = threadStartPoint; x < threadWidth + threadStartPoint; x += step)
			{
				double nX = (x + 0.5) / this->m_width;
				double rMapX = (2 * nX - 1) * aR;
				double camX = rMapX * tan(glm::radians<float>(this->m_camera->getFOV()) / 2);

				glm::vec3 camSpace(camX, camY, -1);

				Ray ray(this->m_camera->getPos(), glm::normalize(camSpace - this->m_camera->getPos()));
				HitInfo hit;

				for (unsigned int i = 0; i < sceneList[this->sceneNumber].size(); i++) {
					sceneList[this->sceneNumber][i]->Intersection(ray, hit);
				}

				if (hit.hitObject != nullptr) {
					Colour finalColour = light.CalculateLighting(
						sceneList[this->sceneNumber],
						ray,
						hit,
						128,
						Colour(glm::vec3(0.2f, 0.2f, 0.2f)),
						Colour(glm::vec3(1.0f, 1.0f, 1.0f)),
						Colour(glm::vec3(1.0f, 1.0f, 1.0f)),
						this->hardShadowsOn,
						this->randShadows,
						this->reflectivenessOn,
						this->areaLight);

					if (!check) {
						this->m_renderSurface->setRect(x, y, step, step, finalColour, this->screenData);
					}
					else {
						this->m_renderSurface->setPixel(x, y, finalColour, this->screenData);
					}
				}
			}
		}

		SDL_BlitSurface(this->m_renderSurface->getSDLRenderSurface(), nullptr, this->m_windowSurface, nullptr);
		SDL_UpdateWindowSurface(m_window);
	}
}

void Window::executeActionSDL(SDL_Event& currentAction)
{
	if (currentAction.type == SDL_QUIT)
	{
		this->SDLWindowRunning = false;
		SDL_Log("SDL has been destroyed, closing action fired.");
	}

	if (currentAction.type == SDL_KEYUP)
	{
		if (this->m_keyboard.isKeyDown((KeyCodes)currentAction.key.keysym.scancode)) {
			this->nrKeyPressed -= 1;
		}
		this->m_keyboard.updateKeystate((KeyCodes)currentAction.key.keysym.scancode, false);
		if (this->nrKeyPressed == 0) {
			this->m_renderSurface->setRenderingState(true);
		}
	}
	if (currentAction.type == SDL_KEYDOWN)
	{
		if (!this->m_keyboard.isKeyDown((KeyCodes)currentAction.key.keysym.scancode)) {
			this->nrKeyPressed += 1;
		}
		this->m_keyboard.updateKeystate((KeyCodes)currentAction.key.keysym.scancode, true);
		this->m_renderSurface->setRenderingState(false);
		this->screenUpdated = false;
		this->frameAction = true;
	}

	if (currentAction.type == SDL_WINDOWEVENT)
	{
		switch (currentAction.window.event)
		{
			case SDL_WINDOWEVENT_SIZE_CHANGED:
			case SDL_WINDOWEVENT_RESIZED:
			{
				this->m_width = currentAction.window.data1;
				this->m_height = currentAction.window.data2;
				this->m_renderSurface->setRenderingState(false);
				break;
			}

			default:
				break;
		}
	}
}

