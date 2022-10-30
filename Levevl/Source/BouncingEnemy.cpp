#include "SDL.h"
#include "Graphics.h"
#include "Sprite.h"
#include "BouncingEnemy.h"
#include "Utilities.h"

BouncingEnemy::BouncingEnemy(int x, int y, Level* level) : Entity(x, y, level), m_sprite(0, 0, 24, 24, 1) {
	m_sprite.SetPosition(m_x, m_y);
}

void BouncingEnemy::Update(Input& input) {

}

void BouncingEnemy::Draw(Graphics& graphics) {
	m_sprite.Draw(graphics, graphics.bouncingEnemyTexture);

}

bool BouncingEnemy::IsRiding(Chunk& chunk) {
	return false;
}