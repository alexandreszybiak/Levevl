#include <algorithm>
#include "SDL.h"
#include "Game.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "Graphics.h"
#include "Input.h"
#include "Chunk.h"
#include "Sprite.h"
#include "Animation.h"

Player::Player(int x, int y) : m_x(x), m_y(y), m_direction(DIRECTION_RIGHT), m_currentBodyAnimation(nullptr), m_onFloor(false), m_boundingBox({ 0, 0, 26, 31 }) {

	m_idleAnimation = new Animation(1);
	m_idleAnimation->PushFrame(5);

	m_runAnimation = new Animation(4);
	m_runAnimation->PushFrame(1);
	m_runAnimation->PushFrame(2); 
	m_runAnimation->PushFrame(3); 
	m_runAnimation->PushFrame(4);

	m_jumpAnimation = new Animation(1);
	m_jumpAnimation->PushFrame(1);

	m_stickIdleAnimation = new Animation(1);
	m_stickIdleAnimation->PushFrame(5);

	m_stickHitAnimation = new Animation(1, false);
	m_stickHitAnimation->PushFrame(6);
	m_stickHitAnimation->PushFrame(5);

	SetAnimation(&m_currentBodyAnimation, m_idleAnimation);
	SetAnimation(&m_currentStickAnimation, m_stickIdleAnimation);

	m_bodyState = new PlayerIdleState();

	m_bodySprite = new Sprite(-17, -17, this, 60, 48, 5);
	m_stickSprite = new Sprite(-17, -17, this, 60, 48, 5);

}

void Player::Update(Input& input) {

	// Updates animations

	int newBodyFrame = m_currentBodyAnimation->Update();
	int newStickFrame = m_currentStickAnimation->Update();

	if (newBodyFrame > -1) {
		m_bodySprite->SetFrame(newBodyFrame);
	}

	if (newStickFrame > -1) {
		m_stickSprite->SetFrame(newStickFrame);
	}

	if (!m_currentStickAnimation->Playing()) {
		SetAnimation(&m_currentStickAnimation, m_stickIdleAnimation);
		m_currentStickAnimation->Reset();
	}

	// Update states

	PlayerState* state = nullptr;
	state = m_bodyState->HandleInput(this, input);
	if (state != NULL) {
		delete m_bodyState;
		m_bodyState = state;
		m_bodyState->Enter(this);
	}

	state = m_bodyState->Update(this);
	if (state != NULL) {
		delete m_bodyState;
		m_bodyState = state;
		m_bodyState->Enter(this);
	}

	if (input.WasKeyPressed(SDL_SCANCODE_X) || input.WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y)) {
		SetAnimation(&m_currentStickAnimation, m_stickHitAnimation);
		m_currentStickAnimation->Reset();
		m_stickSprite->SetFrame(m_currentStickAnimation->GetFrame());

	}

}

void Player::PostUpdate() {
	m_x += m_velocityX;
	m_y += m_velocityY;

	m_boundingBox.x = m_x;
	m_boundingBox.y = m_y;
}

void Player::Draw(Graphics& graphics) {

	m_bodySprite->Draw(graphics, graphics.playerBodyTexture, m_direction);
	m_stickSprite->Draw(graphics, graphics.playerStickTexture, m_direction);

	SDL_RenderDrawRect(graphics.m_renderer, &m_boundingBox);
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
					m_onFloor = true;
					x = 9999;
					break;
				}
			}
			else {
				unoverlappedChunks++;
			}
		}
		;
		if (unoverlappedChunks == chunks.size()) {
			SnapY(bottom);
			m_onFloor = true;
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
}

bool Player::Collide(Chunk& chunk) {
	bool hasCollided = false;
	int bottom = m_y + m_boundingBox.h + m_velocityY;

	if (chunk.OverlapsPoint(m_x, bottom) != 1 || chunk.OverlapsPoint(m_x + m_boundingBox.w - 1, bottom) != 1) {
		m_velocityY = 0.0f;
		m_y = bottom / TILE_SIZE * TILE_SIZE - m_boundingBox.h;
		m_onFloor = true;
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


void Player::SetPosition(int x, int y) {
	m_x = (float)x;
	m_y = (float)y;
	m_velocityY = 0;
}

void Player::SetAnimation(Animation** target, Animation* animation) {
	*target = animation;
	(*target)->Play();
}

void Player::SetState(PlayerState* state) {
	m_bodyState = state;
	m_bodyState->Enter(this);
}