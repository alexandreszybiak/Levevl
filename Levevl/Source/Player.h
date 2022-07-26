#pragma once
#include "SDL.h"

class Graphics;

class Player {
public:

private:
	int m_x, m_y;
	SDL_Rect m_destinationRect;

	int m_currentFrame;

	// The frame duration in milliseconds
	const int m_frameDuration = 83;

	int m_lastFrameTime;

public:
	Player(int x, int y);
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);
};