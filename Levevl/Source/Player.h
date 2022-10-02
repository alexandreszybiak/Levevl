#pragma once
#include <vector>
#include "SDL.h"
#include "Entity.h"
#include "Utilities.h"

#define GRAVITY .48f
#define STICK_TIP_X 20

class Level;
class Graphics;
class Input;
class Chunk;
class PlayerState;
class Sprite;
class Animation;

class Player : public Entity{
	friend class PlayerIdleState;
	friend class PlayerJumpState;
	friend class PlayerFallState;
	friend class PlayerWallSlideState;
	friend class StickIdleState;
	friend class StickHitState;
	friend class StickAimingUpState;
	friend class StickHitUpState;
	friend class StickAimingDownState;
	friend class StickHitDownState;

public:

	// Current animation
	Animation * m_currentBodyAnimation, * m_currentStickAnimation;

	// Body animation
	Animation * m_idleAnimation, * m_runAnimation, * m_jumpAnimation;

	// Stick animation
	Animation * m_stickIdleAnimation, * m_stickHitAnimation, * m_stickIdleUpAnimation, * m_stickHitUpAnimation, * m_stickIdleDownAnimation, * m_stickHitDownAnimation;

private:

	float m_velocityX, m_velocityY;
	float m_xRemainder, m_yRemainder;
	const float m_speed = 2.5f;
	const float m_groundAcceleration = 0.45f;
	const float m_airAcceleration = 0.9f;
	const float m_groundDeceleration = 0.25f;
	const float m_jumpStrength = -9.5f;
	const float m_wallJumpStrength = -4.5f;
	const float m_WallSlideSpeed = 2.0f;
	const float m_WallJumpGravityMultiplier = .45f;

	Sprite * m_bodySprite, * m_stickSprite;

	VerticalLine m_stickCollisionLine;

	AABB m_boundingBox;

	bool m_onFloor;

	// State machines
	PlayerState * m_bodyState, * m_stickState;

	Direction m_direction;

	Level* m_levelRef;

public:
	Player(int x, int y, Level* level);
	void Update(Input& input);
	void Draw(Graphics& graphics);

	// Move and handle collisions
	void MoveX(float x);
	void MoveY(float y);
	bool OnFloor() { return m_onFloor; }
	void SetOnFloor(bool b) { m_onFloor = b; }
	void SetPosition(int x, int y);
	void MoveInstant(int x, int y);
	bool IsRiding(Chunk& chunk);

	void SetAnimation(Animation** target, Animation* animation);
	const Animation* GetAnimation() { return m_currentBodyAnimation; }

	void SetDirection(Direction direction) { m_direction = direction; }
	Direction GetDirection() { return m_direction; }
	void InvertDirection();

private:
	bool HitAtPoint(int x, int y, int dirX, int dirY);

};