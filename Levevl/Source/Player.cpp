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

Player::Player(int x, int y) : Entity(x, y), m_direction(DIRECTION_RIGHT), m_currentBodyAnimation(nullptr), m_onFloor(false), m_stickCollisionLine({ 25,-13,-10 }), m_feetCollisionLine({ 0, -9, 9 }), m_boundingBox({ 0,0,25,25 }) {

	m_idleAnimation = new Animation(1);
	m_idleAnimation->PushFrame(5);

	m_runAnimation = new Animation(4);
	m_runAnimation->PushFrame(1, 1, 0, -2);
	m_runAnimation->PushFrame(2, 1, -1, -1); 
	m_runAnimation->PushFrame(3, 1, 0, -2); 
	m_runAnimation->PushFrame(4, 1, 1, -1);

	m_jumpAnimation = new Animation(1);
	m_jumpAnimation->PushFrame(1, 1, 0, -2);

	m_stickIdleAnimation = new Animation(1);
	m_stickIdleAnimation->PushFrame(5);

	m_stickHitAnimation = new Animation(1, false);
	m_stickHitAnimation->PushFrame(6);
	m_stickHitAnimation->PushFrame(5);

	SetAnimation(&m_currentBodyAnimation, m_idleAnimation);
	SetAnimation(&m_currentStickAnimation, m_stickIdleAnimation);

	m_bodyState = new PlayerIdleState();

	m_bodySprite = new Sprite(-30, -48, this, 60, 48, 5);

	m_stickSocket = new Entity(0, 0, 0, 0, this);

	m_stickSprite = new Sprite(-30, -48, m_stickSocket, 60, 48, 5);

}

void Player::Update(Input& input) {

	// Receive new animation frame

	const KeyFrame* newBodyFrame = m_currentBodyAnimation->Update();
	const KeyFrame* newStickFrame = m_currentStickAnimation->Update();

	if (newBodyFrame) {
		m_bodySprite->SetFrame(newBodyFrame->GetFrameIndex());
		m_stickSocket->SetOrigin(newBodyFrame->GetSocketX(), newBodyFrame->GetSocketY());
	}

	if (newStickFrame) {
		m_stickSprite->SetFrame(newStickFrame->GetFrameIndex());
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
		m_stickSprite->SetFrame(m_currentStickAnimation->GetFrame()->GetFrameIndex());

	}

}

void Player::PostUpdate() {

	m_x += m_velocityX;
	m_y += m_velocityY;

	// CLEAN - make it a child list
	m_bodySprite->Update();
	m_stickSocket->Update();
	m_stickSprite->Update();
}

void Player::Draw(Graphics& graphics) {

	m_bodySprite->Draw(graphics, graphics.playerBodyTexture, m_direction);
	m_stickSprite->Draw(graphics, graphics.playerStickTexture, m_direction);

	//SDL_RenderDrawRect(graphics.m_renderer, &m_boundingBox);

	//int x1 = m_stickCollisionLine.X() * m_direction + m_x;
	//int y1 = m_stickCollisionLine.Start() + m_y;
	//int x2 = m_stickCollisionLine.X() * m_direction + m_x;
	//int y2 = m_stickCollisionLine.End() + m_y;

	int x1 = m_feetCollisionLine.Start() + m_x;
	int y1 = m_feetCollisionLine.Y() + m_y;
	int x2 = m_feetCollisionLine.End() + m_x;
	int y2 = m_feetCollisionLine.Y() + m_y;

	SDL_SetRenderDrawColor(graphics.m_renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(graphics.m_renderer, x1, y1, x2, y2);
}

void Player::Collide(std::vector<Chunk>& chunks) {
	int offset = 0;
	int facing = m_y + m_feetCollisionLine.Y() + m_velocityY;
	if(m_velocityY < 0)
		facing = m_y - 30;

	int x = 0;
	while (1) {
		int unoverlappedChunks = 0;
		for (Chunk& chunk : chunks) {
			int valueAtPoint = chunk.OverlapsPoint(m_x + m_feetCollisionLine.Start() + x, facing);
			if (valueAtPoint) {
				if (valueAtPoint == 2) {
					SnapY(facing, offset);
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
			SnapY(facing, offset);
			m_onFloor = true;
			x = 9999;
			break;
		}
		if (x < m_feetCollisionLine.Length() - 1) {
			x += std::min(m_feetCollisionLine.Length() - 1 - x, TILE_SIZE);
			continue;
		}
		break;
	}

	// Horizontal
	offset = 0;
	facing = m_x + m_stickCollisionLine.X() * m_direction + m_velocityX;
	if (m_velocityX < 0) {
		offset = TILE_SIZE;
	}

	int y = 0;
	while (1) {
		int unoverlappedChunks = 0;
		for (Chunk& chunk : chunks) {
			int valueAtPoint = chunk.OverlapsPoint(facing, m_y + m_stickCollisionLine.Start() + y);
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
		if (y < m_stickCollisionLine.Length() - 1) {
			y += std::min(m_stickCollisionLine.Length() - 1 - y, TILE_SIZE);
			continue;
		}
		break;
	}
}

void Player::SnapX(int point, int offset) {
	int distance = abs(m_x - (point - m_velocityX));
	point = point / TILE_SIZE * TILE_SIZE;
	
	m_x = point + offset + (-distance * m_direction);
	m_velocityX = 0.0f;
}

void Player::SnapY(int point, int offset) {
	int distance = abs(m_y - (point - m_velocityY));
	point = point / TILE_SIZE * TILE_SIZE;
	m_y = point + offset + distance;
	m_velocityY = 0.0f;
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