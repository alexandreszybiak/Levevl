#pragma once
#include <vector>
#include "SDL.h"
#include "Entity.h"
#include "Utilities.h"

#define GRAVITY .48f
#define JUMP_CANCEL_FACTOR .35f

#define HORIZONTAL_STICK_TIP_X 20
#define HORIZONTAL_STICK_TIP_Y1 3
#define HORIZONTAL_STICK_TIP_Y2 4
#define VERTICAL_STICK_TIP_X1 7
#define VERTICAL_STICK_TIP_X2 8
#define VERTICAL_STICK_TIP_Y 25
#define BODY_FRONT_X 12

class Level;
class Graphics;
class Input;
class Chunk;
class PlayerState;

class PlayerIdleState;
class PlayerJumpState;
class PlayerFallState;
class PlayerWallSlideState;
class StickIdleState;
class StickHitState;
class StickAimingUpState;
class StickHitUpState;
class StickAimingDownState;
class StickHitDownState;

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

	// State machines
	PlayerState * m_bodyState, * m_stickState;

	// States
	PlayerIdleState* m_playerIdleState;
	PlayerJumpState* m_playerJumpState;
	PlayerFallState* m_playerFallState;
	PlayerWallSlideState* m_playerWallSlideState;
	StickIdleState* m_stickIdleState;
	StickAimingDownState* m_stickAimingDownState;
	StickAimingUpState* m_stickAimingUpState;
	StickHitState* m_stickHitState;
	StickHitDownState* m_stickHitDownState;
	StickHitUpState* m_stickHitUpState;

	Direction m_direction;

public:
	Player(int x, int y, Level* level);
	void Update(Input& input) override;
	void Draw(Graphics& graphics);

	// Collision checking on the x axis
	bool OverlapsSolidX(int dirX, int offset) override;

	// Collision checking on the y
	bool OverlapsSolidY(int dirY, int offset) override;
	
	// Returns true if the player should be carried by the chunk
	bool IsRiding(Chunk& chunk);

	bool OnFloor() { return m_onFloor; }
	void SetOnFloor(bool b) { m_onFloor = b; }
	void SetPosition(int x, int y);	

	void SetAnimation(Animation** target, Animation* animation);
	const Animation* GetAnimation() { return m_currentBodyAnimation; }

	void SetDirection(Direction direction) { m_direction = direction; }
	Direction GetDirection() { return m_direction; }
	void InvertDirection();

private:
	// The player performs a chunk hit
	bool HitAtPoint(int x, int y, const Vector2& direction);

};