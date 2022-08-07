#include "Entity.h"

void Entity::Update() {
	m_x = m_parent->X() + m_originX;
	m_y = m_parent->Y() + m_originY;
}

void Entity::SetOrigin(int x, int y) {
	m_originX = x;
	m_originY = y;
}