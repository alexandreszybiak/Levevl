#pragma once
#include <vector>
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

	int m_animationIterator;

	int m_currentFrame;

	std::vector<Uint8>* m_currentAnimation;

	// The frame duration in milliseconds
	const int m_frameDuration = 83;

	int m_lastFrameTime;

	// Idle animation
	std::vector<Uint8> m_idleAnimation;

	// Run animation
	std::vector<Uint8> m_runAnimation;

public:
	Player(int x, int y);
	void Update(Input& input);
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);
	void PlayAnimation(std::vector<Uint8>* animation);
};