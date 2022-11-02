#pragma once
#include "Entity.h"
#include "Sprite.h"

class Graphics;
class Input;
class Chunk;
class Level;

class BouncingEnemy : public Entity {
public:
	//
private:
	Sprite m_sprite;
public:
	BouncingEnemy(int x, int y, Level* level);
	void Update(Input& input);
	void Draw(Graphics& graphics) override;

	// Collision checking on the y
	bool OverlapsSolidY(int dirY, int offset) override;

	//bool IsRiding(Chunk& chunk) override;

private:
	//
};