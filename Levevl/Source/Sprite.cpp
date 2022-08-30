#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "Entity.h"
#include "Player.h"
#include "Sprite.h"

void Sprite::Draw(Graphics& graphics, SDL_Texture* texture, Direction direction) {

	if (m_visible == false)
		return;

	SDL_Rect srcRect;
	srcRect.x = m_width * (m_currentFrame % m_numColumn);
	srcRect.y = m_height * (m_currentFrame / m_numColumn);
	srcRect.w = m_width;
	srcRect.h = m_height;

	SDL_Rect destRect;
	destRect.x = m_x - m_pivotX;
	destRect.y = m_y - m_pivotY;
	destRect.w = m_width;
	destRect.h = m_height;

	SDL_RendererFlip flip;
	if (direction == DIRECTION_LEFT)
		flip = SDL_FLIP_HORIZONTAL;
	else if (direction == DIRECTION_RIGHT)
		flip = SDL_FLIP_NONE;

	SDL_Point center;
	center.x = m_pivotX;
	center.y = m_pivotY;

	graphics.Draw(texture, &srcRect, &destRect, flip, m_angle, &center);

}