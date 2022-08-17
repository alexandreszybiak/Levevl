#pragma once
#include <vector>
#include "SDL.h"
#include "Entity.h"
#include "Utilities.h"

#define GRAVITY .45f 

class Level;
class Graphics;
class Input;
class Chunk;
class PlayerState;
class Sprite;
class Animation;

enum Direction {
	DIRECTION_LEFT = -1,
	DIRECTION_RIGHT = 1
};

class Player : public Entity{
	friend class PlayerIdleState;
	friend class PlayerJumpState;

private:
	float m_velocityX, m_velocityY;
	float m_xRemainder, m_yRemainder;
	const float m_speed = 2.5f;

	Sprite* m_bodySprite;
	Sprite* m_stickSprite;

	Entity* m_stickSocket;

	VerticalLine m_stickCollisionLine;

	AABB m_boundingBox;

	bool m_onFloor;

	// States
	PlayerState* m_bodyState;

	Direction m_direction;

	Level* m_levelRef;

public:

	Animation* m_currentBodyAnimation;
	Animation* m_currentStickAnimation;

	Animation* m_idleAnimation;
	Animation* m_stickIdleAnimation;
	Animation* m_runAnimation;
	Animation* m_jumpAnimation;
	Animation* m_stickHitAnimation;

private:
	//

public:
	Player(int x, int y, Level* level);
	void Update(Input& input, Level* level);
	void PostUpdate();
	void Draw(Graphics& graphics);
	void MoveX(float x);
	void MoveY(float y);
	void SetPosition(int x, int y);

	void SetAnimation(Animation** target, Animation* animation);
	const Animation* GetAnimation() { return m_currentBodyAnimation; }

	void SetDirection(Direction direction) { m_direction = direction; }
	Direction GetDirection() { return m_direction; }

	void SetState(PlayerState* state);

	bool OnFloor() { return m_onFloor; }
	void SetOnFloor(bool b) { m_onFloor = b; }

};