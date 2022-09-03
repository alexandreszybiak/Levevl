#pragma once

class Camera {
public:
	int m_x, m_y;
	int m_width, m_height;
private:
	//
public:
	Camera(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height) {}
};