#include "RenderSurface.h"
#include <cstdlib>

RenderSurface::RenderSurface(int width, int height, SDL_Surface* surfaceWindow)
{
	this->m_width = width;
	this->m_height = height;

	this->m_surface = SDL_CreateRGBSurface(
		0,
		width,
		height,
		32,
		surfaceWindow->format->Rmask,
		surfaceWindow->format->Gmask,
		surfaceWindow->format->Bmask,
		surfaceWindow->format->Amask
	);
}

RenderSurface::~RenderSurface()
{
	if (this->m_surface != nullptr)
		SDL_FreeSurface(m_surface);
}

void RenderSurface::Clear(Colour colour)
{
	glm::tvec3<uint8_t> uint8Colour = colour.ToRGB();
	SDL_FillRect(m_surface, nullptr, SDL_MapRGB(this->m_surface->format, uint8Colour.x, uint8Colour.y, uint8Colour.z));
}

void RenderSurface::setPixelSDLMap(int x, int y, int colour)
{
	if (colour >= 0) {
		/* Get a pointer to the video surface's pixels in memory */
		Uint32* pixel = (Uint32*)this->m_surface->pixels;

		/* Calculate offset to the location we wish to write to */
		int offset = y * (this->m_surface->pitch / sizeof(Uint32)) + x;

		*(pixel + offset) = colour;
	}
}


void RenderSurface::setPixel(int x, int y, Colour colour, int** screenData)
{
	/* Get a pointer to the video surface's pixels in memory */
	Uint32* pixel = (Uint32*)this->m_surface->pixels;

	/* Calculate offset to the location we wish to write to */
	int offset = y * (this->m_surface->pitch / sizeof(Uint32)) + x;

	/* Compose RGBA values into correct format for video surface and copy to screen */
	glm::tvec3<uint8_t> uint8Colour = colour.ToRGB();
	Uint32 col = SDL_MapRGB(this->m_surface->format, uint8Colour.x, uint8Colour.y, uint8Colour.z);
	*(pixel + offset) = col;

	if (screenData != 0) {
		screenData[x][y] = col;
	}
}

void RenderSurface::setRect(int x, int y, int width, int height, Colour colour, int** screenData)
{
	glm::tvec3<uint8_t> uint8Colour = colour.ToRGB();
	Uint32 col = SDL_MapRGB(m_surface->format, uint8Colour.x, uint8Colour.y, uint8Colour.z);

	for (int innerPixelx = 0; innerPixelx < width; ++innerPixelx)
	{
		int posPixelx = x + innerPixelx;
		if (posPixelx < 0 || posPixelx >= this->m_width)
			continue;

		for (int innerPixely = 0; innerPixely < height; ++innerPixely)
		{
			int posPixely = y + innerPixely;
			if (posPixely < 0 || posPixely >= this->m_height)
				continue;

			Uint32* pixel = (Uint32*)m_surface->pixels;
			int offset = posPixely * (this->m_surface->pitch / sizeof(Uint32)) + posPixelx;
			*(pixel + offset) = col;

			if (screenData != 0) {
				screenData[posPixelx][posPixely] = col;
			}
		}
	}
}