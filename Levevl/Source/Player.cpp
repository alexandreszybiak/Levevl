#include "SDL.h"
#include "Player.h"
#include "Graphics.h"

Player::Player(int x, int y) :
m_x(x),
m_y(y),
m_destinationRect({0,0,40,48}),
m_currentFrame(0),
m_lastFrameTime(0)
{
	
}

void Player::Draw(Graphics& graphics) {
	if (SDL_GetTicks() - m_lastFrameTime > m_frameDuration) {
		m_currentFrame = (m_currentFrame + 1) % 4;
		m_lastFrameTime = SDL_GetTicks();
	}

	SDL_Rect srcRect = { 40 + 40 * m_currentFrame,0,40,48 };

	m_destinationRect.x = m_x;
	m_destinationRect.y = m_y;

	graphics.Draw(graphics.playerTexture, srcRect, m_destinationRect);
}

void Player::SetPosition(int x, int y) {
	m_x = x;
	m_y = y;
}