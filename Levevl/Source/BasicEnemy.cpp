#include <algorithm>
#include "SDL.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Level.h"
#include "BasicEnemy.h"
#include "Utilities.h"

BasicEnemy::BasicEnemy(int x, int y, Level* level) : Entity(x, y, level), m_sprite(0, 0, 24, 24, 1) {
	m_boundingBox = { -12, -10, 12, 10 };
	m_hurtBox = m_boundingBox;
	m_sprite.SetPosition(m_x, m_y);
}

void BasicEnemy::Update(Input& input) {
	m_velocityY = std::clamp(m_velocityY + .48f, -12.0f, 12.0f);

	MoveX(m_velocityX);
	MoveY(m_velocityY);

	m_sprite.SetPosition(m_x - 12, m_y - 14);
}

void BasicEnemy::Draw(Graphics& graphics) {
	m_sprite.Draw(graphics, graphics.basicEnemyTexture);

	SDL_Rect rect;
	rect.x = m_hurtBox.X1() + m_x;
	rect.y = m_hurtBox.Y1() + m_y;
	rect.w = m_hurtBox.Width();
	rect.h = m_hurtBox.Height();

	SDL_SetRenderDrawColor(graphics.m_renderer, 255, 0, 0, 255);
	graphics.DrawRect(rect);

}