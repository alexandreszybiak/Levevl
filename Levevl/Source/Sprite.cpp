#include "SDL.h"
#include "Graphics.h"
#include "Entity.h"
#include "Player.h"
#include "Sprite.h"

void Sprite::Update() {
	m_x = m_parent->X() + m_originX;
	m_y = m_parent->Y() + m_originY;
}

void Sprite::Draw(Graphics& graphics, SDL_Texture* texture, Direction direction) {
	SDL_Rect srcRect;
	srcRect.x = m_width * (m_currentFrame % m_numColumn);
	srcRect.y = m_height * (m_currentFrame / m_numColumn);
	srcRect.w = m_width;
	srcRect.h = m_height;

	SDL_Rect destRect;
	destRect.x = (int)m_x;
	destRect.y = (int)m_y;
	destRect.w = m_width;
	destRect.h = m_height;

	SDL_RendererFlip flip;
	if (direction == DIRECTION_LEFT)
		flip = SDL_FLIP_HORIZONTAL;
	else if (direction == DIRECTION_RIGHT)
		flip = SDL_FLIP_NONE;

	graphics.Draw(texture, &srcRect, &destRect, flip);
}