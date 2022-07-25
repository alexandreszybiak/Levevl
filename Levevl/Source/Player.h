#pragma once
#include "SDL.h"

class Graphics;

class Player {
public:

private:
	int m_x, m_y;
	SDL_Rect m_destinationRect;

public:
	Player(int x, int y);
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);
};