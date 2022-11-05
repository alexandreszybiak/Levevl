#pragma once
#include "Utilities.h"

#define ENTITY_MAX_SPEED 24.0f

class Graphics;
class Input;
class Chunk;
class Level;

class Entity {
public:
	int m_index;

protected:
	int m_x, m_y;
	float m_velocityX, m_velocityY;
	bool m_onFloor;
	float m_xRemainder, m_yRemainder;
	int m_carryAmountX, m_carryAmountY;
	float m_bounciness;

	AABB m_boundingBox;

	Level* m_levelRef;

public:
	/*Entity() : m_x(0), m_y(0), m_xRemainder(.0f), m_yRemainder(.0f), m_carryAmountX(0), m_carryAmountY(0), m_index(-1), m_levelRef(nullptr) {}*/
	Entity(int x, int y, Level* level) : m_x(x), m_y(y), m_velocityX(.0f), m_velocityY(.0f), m_onFloor(false), m_xRemainder(.0f), m_yRemainder(.0f), m_carryAmountX(0), m_carryAmountY(0), m_bounciness(.0f), m_index(-1), m_levelRef(level) {}

	virtual void Update(Input& input) = 0;

	virtual void Draw(Graphics& graphics) = 0;

	// Move by integer values on the x axis and stop at solid
	virtual void MoveX(float x);

	// Move by integer values on the y axis and stop at solid
	virtual void MoveY(float y);

	virtual void MoveInstant(int x, int y);

	// Preparation for carry
	void SetCarryAmount(int x, int y);

	// Carry the entity
	void Carry();

	// Collision checking on the x axis
	virtual bool OverlapsSolidX(int dirX, int offset);

	// Collision checking on the y
	virtual bool OverlapsSolidY(int dirY, int offset);

	// Check if that entity is riding that chunk
	virtual bool IsRiding(Chunk& chunk);


	int X() { return m_x; }
	int Y() { return m_y; }
	
};