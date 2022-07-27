#pragma once
#include "SDL.h"

class Graphics;
class Input;

enum Direction {
	DIRECTION_LEFT = -1,
	DIRECTION_RIGHT = 1
};

class Player {
public:

private:
	int m_x, m_y;

	Direction m_direction;

	SDL_Rect m_destinationRect;

	int m_currentFrame;

	// The frame duration in milliseconds
	const int m_frameDuration = 83;

	int m_lastFrameTime;

public:
	Player(int x, int y);
	void Update(Input& input);
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);
};