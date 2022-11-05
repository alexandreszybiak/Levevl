#include <algorithm>
#include <math.h>
#include "Game.h"
#include "Utilities.h"
#include "Level.h"
#include "Entity.h"
#include "Chunk.h"

void Entity::MoveX(float x) {
	m_xRemainder += x;
	int move = (int)m_xRemainder;

	if (move == 0)
		return;

	m_xRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {

		if (OverlapsSolidX(sign, sign)) {
			m_velocityX = m_velocityX * m_bounciness * -1.0f;
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
			m_velocityY = m_velocityY * m_bounciness * -1.0f;
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
	VerticalLine bodyLine = { m_x + m_boundingBox.X2() * dirX + offset, m_y + m_boundingBox.Y1(), m_y + m_boundingBox.Y2() };

	if (m_levelRef->LineOverlapsSolid(bodyLine)) {
		return true;
	}

	return false;
}

bool Entity::OverlapsSolidY(int dirY, int offset) {
	HorizontalLine bodyLine = { m_y + m_boundingBox.Y2() * dirY + offset, m_x + m_boundingBox.X1(), m_x + m_boundingBox.X2() };

	if (m_levelRef->LineOverlapsSolid(bodyLine)) {
		return true;
	}

	return false;
}

bool Entity::IsRiding(Chunk& chunk) {
	HorizontalLine feetLine = { m_y + m_boundingBox.Y2(), m_x + m_boundingBox.X1(), m_x + m_boundingBox.X2() };

	int y = feetLine.Y();
	int x = feetLine.Start();

	// Horizontal
	while (1) {
		bool overlapsAtFeetPoint = chunk.OverlapsPoint(Vector2(x, y));
		bool solidAtUnderFeetPoint = chunk.SolidAtPoint(Vector2(x, y + 1));
		if (overlapsAtFeetPoint && solidAtUnderFeetPoint) {
			return true;
		}
		if (x < feetLine.End() - 1) {
			x += std::min(feetLine.End() - 1 - x, TILE_SIZE);
			continue;
		}
		break;
	}

	return false;


}