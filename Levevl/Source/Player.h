#pragma once
#include <vector>
#include "SDL.h"

#define GRAVITY .5f 

class Graphics;
class Input;
class Chunk;

enum Direction {
	DIRECTION_LEFT = -1,
	DIRECTION_RIGHT = 1
};

class Player {
public:

private:
	float m_x, m_y, m_velocityX, m_velocityY;

	const int m_width = 40;
	const int m_height = 48;

	const SDL_Rect m_boundingBox = { 11,22, 18, 26 };

	bool m_onFLoor;

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
	void PostUpdate();
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);
	void PlayAnimation(std::vector<Uint8>* animation);
	bool Collide();
	void Collide(std::vector<Chunk>& chunks);
	bool Collide(Chunk& chunk);

	bool CheckCollisionY(Chunk& chunk);

	void SnapX(int point, int offset);
	void SnapY(int point);
};