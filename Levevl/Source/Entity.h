#pragma once

class Entity {
protected:
	int m_x, m_y;
	int m_originX, m_originY;
	Entity* m_parent;
public:
	Entity() : m_x(0), m_y(0), m_originX(0), m_originY(0), m_parent(nullptr) {}
	Entity(int x, int y) : m_x(x), m_y(y), m_originX(0), m_originY(0), m_parent(nullptr) {}
	Entity(int x, int y, int originX, int originY, Entity* parent) : m_x(x), m_y(y), m_originX(originX), m_originY(originY), m_parent(parent) {}

	virtual void Update();

	int X() { return m_x; }
	int Y() { return m_y; }

	void SetOrigin(int x, int y);
	
};