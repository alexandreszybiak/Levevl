#include <algorithm>
#include "SDL.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Level.h"
#include "BouncingEnemy.h"
#include "Utilities.h"

BouncingEnemy::BouncingEnemy(int x, int y, Level* level) : Entity(x, y, level), m_sprite(0, 0, 24, 24, 1) {
	m_boundingBox = { 0, 0, 24, 24 };
	m_sprite.SetPosition(m_x, m_y);
}

void BouncingEnemy::Update(Input& input) {
	m_velocityY = std::clamp(m_velocityY + .48f, -12.0f, 12.0f);

	MoveX(m_velocityX);
	MoveY(m_velocityY);

	m_sprite.SetPosition(m_x, m_y);
}

void BouncingEnemy::Draw(Graphics& graphics) {
	m_sprite.Draw(graphics, graphics.bouncingEnemyTexture);

}

bool BouncingEnemy::OverlapsSolidY(int dirY, int offset) {
	HorizontalLine bodyLine = { m_y + m_boundingBox.Y2() * dirY + offset, m_x + m_boundingBox.X1(), m_x + m_boundingBox.X2() };

	if (m_levelRef->LineOverlapsSolid(bodyLine)) {
		return true;
	}

	return false;
}

//bool BouncingEnemy::IsRiding(Chunk& chunk) {
//	return false;
//}