#include <algorithm>
#include "Utilities.h"
#include "Entity.h"

void Entity::MoveX(float x) {
	m_xRemainder += x;
	int move = (int)m_xRemainder;

	if (move == 0)
		return;

	m_xRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {

		if (OverlapsSolidX(sign, sign)) {
			m_velocityX = .0f;
			break;
		}

		m_x += sign;
		move -= sign;

	}
}

void Entity::MoveY(float y) {
	m_yRemainder += y;
	int move = round(m_yRemainder);

	if (move == 0)
		return;

	m_yRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {

		if (OverlapsSolidY(sign, sign)) {
			m_velocityY = .0f;
			if (move > 0) {
				m_onFloor = true;
			}
			break;
		}

		m_y += sign;
		move -= sign;
	}
}

void Entity::MoveInstant(int x, int y) {
	m_x += x;
	m_y += y;
}

void Entity::Carry() {
	MoveX(m_carryAmountX);
	MoveY(m_carryAmountY);
	m_carryAmountX = 0;
	m_carryAmountY = 0;
}

void Entity::SetCarryAmount(int x, int y) {
	m_carryAmountX = x;
	m_carryAmountY = y;
}

bool Entity::OverlapsSolidX(int dirX, int offset) {
	return false;
}

bool Entity::OverlapsSolidY(int dirY, int offset) {
	return false;
}