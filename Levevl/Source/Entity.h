#pragma once

class Entity {
protected:
	float m_x, m_y;
	Entity* m_parent;
public:
	Entity() : m_x(0), m_y(0), m_parent(nullptr) {}
	Entity(float x, float y) : m_x(x), m_y(y), m_parent(nullptr) {}
	Entity(float x, float y, Entity* parent) : m_x(x), m_y(y), m_parent(parent) {}

	virtual void Update();

	float X() { return m_x; }
	float Y() { return m_y; }
	
};