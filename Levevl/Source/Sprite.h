#pragma once

class Player;
class Graphics;
struct SDL_Texture;

class Sprite : public Entity {
private:
	int m_originX, m_originY, m_width, m_height, m_numColumn, m_currentFrame;

public:
	Sprite(float x, float y, Entity* parent, int width, int height, int numColumn) : Entity(x, y, parent), m_originX(x),  m_originY(y), m_width(width), m_height(height), m_numColumn(numColumn), m_currentFrame(1) {}

	void Update();

	void Draw(Graphics& graphics, SDL_Texture* texture, Direction direction);
	void SetFrame(int frame) { m_currentFrame = frame; }
};