#pragma once
#include <vector>
#include "SDL.h"
#include "Entity.h"
#include "Utilities.h"

#define GRAVITY .5f 

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
public:

	float m_velocityX, m_velocityY;

	Animation* m_currentBodyAnimation;
	Animation* m_currentStickAnimation;

	Animation* m_idleAnimation;
	Animation* m_stickIdleAnimation;
	Animation* m_runAnimation;
	Animation* m_jumpAnimation;
	Animation* m_stickHitAnimation;


private:
	Sprite* m_bodySprite;
	Sprite* m_stickSprite;

	Entity* m_stickSocket;

	VerticalLine m_stickCollisionLine;
	HorizontalLine m_feetCollisionLine;

	SDL_Rect m_boundingBox;

	bool m_onFloor;

	// States
	PlayerState* m_bodyState;

	Direction m_direction;



public:
	Player(int x, int y);
	void Update(Input& input);
	void PostUpdate();
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);

	void SetAnimation(Animation** target, Animation* animation);
	const Animation* GetAnimation() { return m_currentBodyAnimation; }

	void Collide(std::vector<Chunk>& chunks);

	void SnapX(int point, int offset);
	void SnapY(int point, int offset);

	void SetDirection(Direction direction) { m_direction = direction; }
	Direction GetDirection() { return m_direction; }

	void SetState(PlayerState* state);

	bool OnFloor() { return m_onFloor; }
	void SetOnFloor(bool b) { m_onFloor = b; }
};