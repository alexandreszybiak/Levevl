#include "Entity.h"

void Entity::Update() {
	m_x = m_parent->X();
	m_y = m_parent->Y();
}