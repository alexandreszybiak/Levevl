#include "SDL.h"
#include "Graphics.h"
#include "Player.h"
#include "Sprite.h"

void Sprite::Draw(Graphics& graphics, SDL_Texture* texture, Direction direction) {
	SDL_Rect srcRect;
	srcRect.x = m_width * (m_currentFrame % m_numColumn);
	srcRect.y = m_height * (m_currentFrame / m_numColumn);
	srcRect.w = m_width;
	srcRect.h = m_height;

	SDL_Rect destRect;
	destRect.x = (int)m_parent->m_x + m_x;
	destRect.y = (int)m_parent->m_y + m_y;
	destRect.w = m_width;
	destRect.h = m_height;

	SDL_RendererFlip flip;
	if (direction == DIRECTION_LEFT)
		flip = SDL_FLIP_HORIZONTAL;
	else if (direction == DIRECTION_RIGHT)
		flip = SDL_FLIP_NONE;

	graphics.Draw(texture, &srcRect, &destRect, flip);
}