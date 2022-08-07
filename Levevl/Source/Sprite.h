#pragma once

class Player;
class Graphics;
struct SDL_Texture;

class Sprite {
private:
	int m_x, m_y, m_width, m_height, m_numColumn, m_currentFrame;
	Player* m_parent;

public:
	Sprite(int x, int y, Player* parent, int width, int height, int numColumn) : m_x(x), m_y(y), m_parent(parent), m_width(width), m_height(height), m_numColumn(numColumn), m_currentFrame(1) {}
	void Draw(Graphics& graphics, SDL_Texture* texture, Direction direction);
	void SetFrame(int frame) { m_currentFrame = frame; }
};