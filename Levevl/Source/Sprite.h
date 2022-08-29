#pragma once

class Player;
class Graphics;
struct SDL_Texture;

class Sprite {
private:
	int m_x, m_y, m_width, m_height, m_numColumn, m_currentFrame;

public:
	Sprite(int x, int y, int width, int height, int numColumn) : m_x(x), m_y(y), m_width(width), m_height(height), m_numColumn(numColumn), m_currentFrame(1) {}

	void Draw(Graphics& graphics, SDL_Texture* texture, Direction direction = DIRECTION_RIGHT, double angle = 0);
	void SetFrame(int frame) { m_currentFrame = frame; }
	void SetPosition(int x, int y) { m_x = x, m_y = y; }
};