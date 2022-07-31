#include <algorithm>
#include "SDL.h"
#include "Game.h"
#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "Chunk.h"

Player::Player(int x, int y) : m_x(x), m_y(y), m_direction(DIRECTION_RIGHT), m_destinationRect({ 0,0,m_width,m_height }), m_currentFrame(0), m_lastFrameTime(0), m_currentAnimation(nullptr), m_onFLoor(false) {

	m_idleAnimation.reserve(1);
	m_idleAnimation = { 0 };

	m_runAnimation.reserve(4);
	m_runAnimation = { 1, 2, 3, 4 };

	PlayAnimation(&m_idleAnimation);

}

void Player::Update(Input& input) {
	int hDir = -(input.IsKeyHeld(SDL_SCANCODE_LEFT) || input.IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) + (input.IsKeyHeld(SDL_SCANCODE_RIGHT) || input.IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));

	if (m_onFLoor && (input.WasKeyPressed(SDL_SCANCODE_UP) || input.WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_A))) {
		m_velocityY = -8;
		m_onFLoor = false;
	}

	if (hDir != 0) {
		PlayAnimation(&m_runAnimation);
		if (hDir > 0)
			m_direction = DIRECTION_RIGHT;
		else if (hDir < 0)
			m_direction = DIRECTION_LEFT;
	}
	else {
		PlayAnimation(&m_idleAnimation);
	}

	m_velocityX = hDir * 2;
	m_velocityY = std::clamp(m_velocityY + GRAVITY, -12.0f, 12.0f);

}

bool Player::Collide() {
	return false;
}

bool Player::CheckCollisionY(Chunk& chunk) {
	return false;
}

void Player::Collide(std::vector<Chunk>& chunks) {
	
	int bottom = m_y + m_boundingBox.h + m_velocityY;
	
	int x = 0;
	while (1) {
		int unoverlappedChunks = 0;
		for (Chunk& chunk : chunks) {
			int valueAtPoint = chunk.OverlapsPoint(m_x + x, bottom);
			if (valueAtPoint) {
				if (valueAtPoint == 2) {
					SnapY(bottom);
					x = 9999;
					break;
				}
			}
			else {
				unoverlappedChunks++;
			}
		}
		;
		if (unoverlappedChunks == chunks.size()){
			SnapY(bottom);
			x = 9999;
			break;
		}
		if (x < m_boundingBox.w - 1) {
			x += std::min(m_boundingBox.w - 1 - x, TILE_SIZE);
			continue;
		}
		break;
	}

	
	// Horizontal
	int offset = -m_boundingBox.w;
	int facing;
	if (m_velocityX < 0) {
		facing = m_x + m_velocityX;
		offset = TILE_SIZE;
	}

	else
		facing = m_x + m_boundingBox.w + m_velocityX;

	int y = 0;
	while (1) {
		int unoverlappedChunks = 0;
		for (Chunk& chunk : chunks) {
			int valueAtPoint = chunk.OverlapsPoint(facing, m_y + y);
			if (valueAtPoint) {
				if (valueAtPoint == 2) {
					SnapX(facing, offset);
					y = 9999;
					break;
				}
			}
			else {
				unoverlappedChunks++;
			}
		}
		;
		if (unoverlappedChunks == chunks.size()) {
			SnapX(facing, offset);
			y = 9999;
			break;
		}
		if (y < m_boundingBox.h - 1) {
			y += std::min(m_boundingBox.h - 1 - y, TILE_SIZE);
			continue;
		}
		break;
	}
}

void Player::SnapX(int point, int offset) {
	m_velocityX = 0.0f;
	m_x = point / TILE_SIZE * TILE_SIZE + offset;
}

void Player::SnapY(int point) {
	m_velocityY = 0.0f;
	m_y = point / TILE_SIZE * TILE_SIZE - m_boundingBox.h;
	m_onFLoor = true;
}

bool Player::Collide(Chunk& chunk) {
	bool hasCollided = false;
	int bottom = m_y + m_boundingBox.h + m_velocityY;

	if (chunk.OverlapsPoint(m_x, bottom) != 1 || chunk.OverlapsPoint(m_x + m_boundingBox.w - 1, bottom) != 1) {
		m_velocityY = 0.0f;
		m_y = bottom / TILE_SIZE * TILE_SIZE - m_boundingBox.h;
		m_onFLoor = true;
		hasCollided = true;
	}

	int offset = -m_boundingBox.w;
	int facing;
	if (m_velocityX < 0) {
		facing = m_x + m_velocityX;
		offset = TILE_SIZE;
	}
		
	else
		facing = m_x + m_boundingBox.w + m_velocityX;

	if (chunk.OverlapsPoint(facing, m_y) != 1 || chunk.OverlapsPoint(facing, m_y + m_boundingBox.h - 1) != 1) {
		m_velocityX = 0.0f;
		m_x = facing / TILE_SIZE * TILE_SIZE + offset;
		hasCollided = true;
	}

	return false;
}

void Player::PostUpdate() {
	m_x += m_velocityX;
	m_y += m_velocityY;
}

void Player::Draw(Graphics& graphics) {
	

	if (SDL_GetTicks() - m_lastFrameTime > m_frameDuration) {
		m_animationIterator = (m_animationIterator + 1) % m_currentAnimation->size();
		m_currentFrame = (*m_currentAnimation)[m_animationIterator];
		m_lastFrameTime = SDL_GetTicks();
	}

	SDL_Rect srcRect = { m_width * m_currentFrame,0,m_width,m_height };

	m_destinationRect.x = (int)m_x - m_boundingBox.x;
	m_destinationRect.y = (int)m_y - m_boundingBox.y;

	/*m_destinationRect.x = m_x;
	m_destinationRect.y = m_y;
	m_destinationRect.w = m_boundingBox.w;
	m_destinationRect.h = m_boundingBox.h;

	SDL_RenderFillRect(graphics.m_renderer, &m_destinationRect);
	return;*/

	SDL_RendererFlip flip;
	if (m_direction == DIRECTION_LEFT)
		flip = SDL_FLIP_HORIZONTAL;
	else if (m_direction == DIRECTION_RIGHT)
		flip = SDL_FLIP_NONE;

	graphics.Draw(graphics.playerTexture, srcRect, m_destinationRect, flip);
}

void Player::SetPosition(int x, int y) {
	m_x = (float)x;
	m_y = (float)y;
	m_velocityY = 0;
}

void Player::PlayAnimation(std::vector<Uint8>* animation) {
	m_currentAnimation = animation;
}