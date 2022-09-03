#pragma once

class Player;
class Graphics;
struct SDL_Texture;

class Sprite {
private:
	int m_x, m_y, m_width, m_height, m_numColumn, m_currentFrame, m_pivotX, m_pivotY;
	bool m_visible;
	double m_angle;

public:
	Sprite(int x, int y, int width, int height, int numColumn, int pivotX = 0, int pivotY = 0 ) : m_x(x), m_y(y), m_width(width), m_height(height), m_numColumn(numColumn), m_currentFrame(1), m_pivotX(pivotX), m_pivotY(pivotY), m_visible(true), m_angle(.0f) {}

	void Draw(Graphics& graphics, SDL_Texture* texture, Direction direction = DIRECTION_RIGHT);
	void SetFrame(int frame) { m_currentFrame = frame; }
	void SetPosition(int x, int y) { m_x = x, m_y = y; }
	void SetVisible(bool visible) { m_visible = visible; }
	void SetAngle(double angle) { m_angle = angle; }
	void Move(int x, int y) { m_x += x; m_y += y; }
};