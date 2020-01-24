#pragma once
#include "SDL2-2.0.4/include/SDL.h"
#include <string>
#include "../Window/RenderSurface.h"
#include "../Utils/Camera.h"
#include "../Utils/Keyboard.h"

class Window {
private:
	SDL_Window* m_window = nullptr;
	SDL_Surface* m_windowSurface = nullptr;
	RenderSurface* m_renderSurface = nullptr;

	Camera* m_camera = nullptr;
	Keyboard m_keyboard;
	int nrKeyPressed = 0;

	int** screenData;
	bool screenUpdated = false;
	bool frameAction = true;
	int sceneNumber = 0;

	bool hardShadowsOn = false;
	bool reflectivenessOn = false;
	bool areaLight = false;
	bool randShadows = false;

	std::string m_title;
	int m_width;
	int m_height;
	int m_frameRate;

	bool SDLWindowRunning;

	void InitSDL_API();
	void DestroySDL_API();
	void generateScreen();
	void executeActionSDL(SDL_Event& currentAction);

public:
	Window(std::string title, int width, int height, int frameRate = 30);
	~Window();

	void initSDL(Camera* camera);

	// TO DO RE DO THE WAY IT"S CHECKING FOR MAPPED SCENE
	void ThreadDrawer(int threadStartPoint, int threadWidth, bool check);

	inline std::string getTitle() const { return m_title; }
	inline int getWidth() const { return m_width; }
	inline int getHeight() const { return m_height; }
	inline RenderSurface* getRenderSurface() const { return m_renderSurface; }
};