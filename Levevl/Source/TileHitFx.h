#pragma once

class Graphics;
class Animation;
class Sprite;

class TileHitFx {
public:

private:

	Animation* m_animation;
	Sprite& m_sprite;

public:
	TileHitFx();
	~TileHitFx();
	void Update();
	void Draw(Graphics& graphics);
	void SetPosition(int x, int y);
	void Reset(int x, int y, int dirX, int dirY);
	void Move(int x, int y);
};