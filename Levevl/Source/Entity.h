#pragma once

#define ENTITY_MAX_SPEED 24.0f

class Graphics;
class Input;
class Chunk;

class Entity {
protected:
	int m_x, m_y;
	float m_xRemainder, m_yRemainder;
	int m_carryAmountX, m_carryAmountY;

public:
	Entity() : m_x(0), m_y(0), m_xRemainder(.0f), m_yRemainder(.0f), m_carryAmountX(0), m_carryAmountY(0) {}
	Entity(int x, int y) : m_x(x), m_y(y), m_xRemainder(.0f), m_yRemainder(.0f), m_carryAmountX(0), m_carryAmountY(0) {}

	virtual void Update(Input& input) = 0;

	virtual void Draw(Graphics& graphics) = 0;

	// Move by integer values on the x axis, override this function if you want collision detection and resolution
	virtual void MoveX(float x);

	// Move by integer values on the y axis, override this function if you want collision detection and resolution
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
	virtual bool IsRiding(Chunk& chunk) = 0;


	int X() { return m_x; }
	int Y() { return m_y; }
	
};