#pragma once
#include <vector>
#include "SDL.h"

#define GRAVITY .5f 

class Graphics;
class Input;
class Chunk;
class PlayerState;

enum Direction {
	DIRECTION_LEFT = -1,
	DIRECTION_RIGHT = 1
};

class Player {
public:
	float m_x, m_y, m_velocityX, m_velocityY;


	// Idle animation
	std::vector<Uint8> m_idleAnimation;

	// Run animation
	std::vector<Uint8> m_runAnimation;

	std::vector<Uint8> m_jumpAnimation;

	std::vector<Uint8> m_hitAnimation;

private:
	

	const int m_width = 60;
	const int m_height = 48;

	const int m_numColumn = 5;

	const SDL_Rect m_boundingBox = { 17,17, 26, 31 };

	bool m_onFloor;

	// States
	PlayerState* m_bodyState;

	Direction m_direction;

	SDL_Rect m_destinationRect;

	int m_animationIterator;

	int m_currentFrame;

	std::vector<Uint8>* m_currentAnimation;

	// The frame duration in milliseconds
	const int m_frameDuration = 83;

	int m_lastFrameTime;


public:
	Player(int x, int y);
	void Update(Input& input);
	void PostUpdate();
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);

	void PlayAnimation(std::vector<Uint8>* animation);
	std::vector<Uint8>* GetAnimation() { return m_currentAnimation; }

	bool Collide();
	void Collide(std::vector<Chunk>& chunks);
	bool Collide(Chunk& chunk);

	bool CheckCollisionY(Chunk& chunk);

	void SnapX(int point, int offset);
	void SnapY(int point);

	void SetDirection(Direction direction) { m_direction = direction; }
	Direction GetDirection() { return m_direction; }

	void SetState(PlayerState* state);

	bool OnFloor() { return m_onFloor; }
	void SetOnFloor(bool b) { m_onFloor = b; }
};