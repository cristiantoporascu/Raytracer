#pragma once
#include "SDL2-2.0.4/include/SDL.h"
#include "../Base/Colour.h"

class RenderSurface
{
private:
	SDL_Surface* m_surface = nullptr;

	int m_width;
	int m_height;

	bool m_renderingActive = true;

public:
	RenderSurface(int width, int height, SDL_Surface* surfaceWindow);
	~RenderSurface();

	void Clear(Colour colour);
	void setPixelSDLMap(int x, int y, int colour);
	void setPixel(int x, int y, Colour colour, int** screenData = 0);
	void setRect(int x, int y, int width, int height, Colour colour, int** screenData = 0);
	void setRenderingState(bool newState) { this->m_renderingActive = newState; }

	inline SDL_Surface* getSDLRenderSurface() const { return this->m_surface; }
	inline int getWidth() const { return this->m_width; }
	inline int getHeight() const { return this->m_height; }
	inline int getRenderingState() const { return this->m_renderingActive; }
};

