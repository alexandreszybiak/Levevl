#include "SDL.h"
#include "Player.h"
#include "Graphics.h"
#include "Input.h"

Player::Player(int x, int y) : m_x(x), m_y(y), m_direction(DIRECTION_RIGHT), m_destinationRect({0,0,40,48}),
	m_currentFrame(0), m_lastFrameTime(0) {
	
}

void Player::Update(Input& input) {
	int hDir = -input.IsKeyHeld(SDL_SCANCODE_LEFT) + input.IsKeyHeld(SDL_SCANCODE_RIGHT);

	if (hDir > 0)
		m_direction = DIRECTION_RIGHT;
	else if (hDir < 0)
		m_direction = DIRECTION_LEFT;

	m_x += hDir * 2;
}

void Player::Draw(Graphics& graphics) {
	if (SDL_GetTicks() - m_lastFrameTime > m_frameDuration) {
		m_currentFrame = (m_currentFrame + 1) % 4;
		m_lastFrameTime = SDL_GetTicks();
	}

	SDL_Rect srcRect = { 40 + 40 * m_currentFrame,0,40,48 };

	m_destinationRect.x = m_x;
	m_destinationRect.y = m_y;

	SDL_RendererFlip flip;
	if (m_direction == DIRECTION_LEFT)
		flip = SDL_FLIP_HORIZONTAL;
	else if (m_direction == DIRECTION_RIGHT)
		flip = SDL_FLIP_NONE;

	graphics.Draw(graphics.playerTexture, srcRect, m_destinationRect, flip);
}

void Player::SetPosition(int x, int y) {
	m_x = x;
	m_y = y;
}