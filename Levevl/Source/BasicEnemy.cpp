#include <algorithm>
#include "SDL.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Level.h"
#include "BasicEnemy.h"
#include "Utilities.h"

BasicEnemy::BasicEnemy(int x, int y, Level* level) : Entity(x, y, level), m_sprite(0, 0, 24, 24, 1) {
	//m_bounciness = 0.25f;
	m_boundingBox = { -12, -10, 12, 10 };
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

	/*SDL_Rect rect;
	rect.x = m_boundingBox.X1() + m_x;
	rect.y = m_boundingBox.Y1() + m_y;
	rect.w = m_boundingBox.Width();
	rect.h = m_boundingBox.Height();

	SDL_SetRenderDrawColor(graphics.m_renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(graphics.m_renderer, &rect);*/

}