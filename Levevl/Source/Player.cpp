#include "Player.h"
#include "Graphics.h"

Player::Player(int x, int y) : m_x(x), m_y(y), m_destinationRect({0,0,26,39}) {}

void Player::Draw(Graphics& graphics) {
	SDL_Rect srcRect = { 0,0,26,39 };

	m_destinationRect.x = m_x;
	m_destinationRect.y = m_y;

	graphics.Draw(graphics.playerTexture, srcRect, m_destinationRect);
}

void Player::SetPosition(int x, int y) {
	m_x = x;
	m_y = y;
}