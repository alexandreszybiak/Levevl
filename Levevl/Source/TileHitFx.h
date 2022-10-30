#pragma once
#include "Entity.h"

class Input;
class Graphics;
class Animation;
class Sprite;
class Level;

class TileHitFx : public Entity {
public:

private:

	Animation* m_animation;
	Sprite& m_sprite;

public:
	TileHitFx(Level* level);
	~TileHitFx();
	void Update(Input& input) override;
	void Draw(Graphics& graphics);

	// Check if that entity is riding that chunk
	bool IsRiding(Chunk& chunk) override;

	void SetPosition(int x, int y);
	void Reset(int x, int y, int dirX, int dirY);
	void Move(int x, int y);
};