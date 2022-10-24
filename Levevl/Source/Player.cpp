#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "Game.h"
#include "TileMap.h"
#include "Level.h"
#include "Player.h"
#include "PlayerStates.h"
#include "Graphics.h"
#include "Input.h"
#include "Sprite.h"
#include "Animation.h"
#include "Chunk.h"
#include "TileHitFx.h"

Player::Player(int x, int y, Level* level) : Entity(x, y), m_direction(DIRECTION_RIGHT), m_currentBodyAnimation(nullptr), m_onFloor(false), m_stickCollisionLine({ 25,4,5 }), m_boundingBox({ -12,-16,12,16 }), m_levelRef(level) {

	// Init animations

	m_idleAnimation = new Animation(1);
	m_idleAnimation->PushFrame(5);

	m_runAnimation = new Animation(4);
	m_runAnimation->PushFrame(1, 1);
	m_runAnimation->PushFrame(2, 1); 
	m_runAnimation->PushFrame(3, 1); 
	m_runAnimation->PushFrame(4, 1);

	m_jumpAnimation = new Animation(1);
	m_jumpAnimation->PushFrame(1, 1);

	m_stickIdleAnimation = new Animation(1);
	m_stickIdleAnimation->PushFrame(5);

	m_stickIdleUpAnimation = new Animation(1);
	m_stickIdleUpAnimation->PushFrame(7);

	m_stickHitUpAnimation = new Animation(1, false);
	m_stickHitUpAnimation->PushFrame(8);

	m_stickIdleDownAnimation = new Animation(1);
	m_stickIdleDownAnimation->PushFrame(8);

	m_stickHitDownAnimation = new Animation(1, false);
	m_stickHitDownAnimation->PushFrame(7);

	m_stickHitAnimation = new Animation(1, false);
	m_stickHitAnimation->PushFrame(6);
	m_stickHitAnimation->PushFrame(5);

	SetAnimation(&m_currentBodyAnimation, m_idleAnimation);
	SetAnimation(&m_currentStickAnimation, m_stickIdleAnimation);

	// Init player states

	m_playerIdleState = new PlayerIdleState();
	m_playerJumpState = new PlayerJumpState();
	m_playerFallState = new PlayerFallState();
	m_playerWallSlideState = new PlayerWallSlideState();

	m_bodyState = m_playerIdleState;

	// Init stick states

	m_stickIdleState = new StickIdleState();
	m_stickAimingDownState = new StickAimingDownState();
	m_stickAimingUpState = new StickAimingUpState();
	m_stickHitState = new StickHitState();
	m_stickHitDownState = new StickHitDownState();
	m_stickHitUpState = new StickHitUpState();

	m_stickState = m_stickIdleState;

	// Init sprites

	m_bodySprite = new Sprite(-30, -40, 60, 56, 5);
	m_stickSprite = new Sprite(-30, -40, 60, 56, 5);

}

void Player::Update(Input& input) {

	// Receive new animation frame

	const KeyFrame* newBodyFrame = m_currentBodyAnimation->Update();
	const KeyFrame* newStickFrame = m_currentStickAnimation->Update();

	if (newBodyFrame) {
		m_bodySprite->SetFrame(newBodyFrame->GetFrameIndex());
	}

	if (newStickFrame) {
		m_stickSprite->SetFrame(newStickFrame->GetFrameIndex());
	}

	// Update states

	PlayerState* state = nullptr;

	state = m_bodyState->HandleInput(this, input);
	if (state != NULL) {
		m_bodyState = state;
		m_bodyState->Enter(this);
	}

	state = m_stickState->HandleInput(this, input);
	if (state != NULL) {
		m_stickState = state;
		m_stickState->Enter(this);
	}

	state = m_bodyState->Update(this);
	if (state != NULL) {
		m_bodyState = state;
		m_bodyState->Enter(this);
	}

	state = m_stickState->Update(this);
	if (state != NULL) {
		m_stickState = state;
		m_stickState->Enter(this);
	}

	// Move player

	MoveX(m_velocityX);
	MoveY(m_velocityY);

	// Update sprite position

	m_bodySprite->SetPosition(m_x - 30, m_y - 40);
	m_stickSprite->SetPosition(m_x - 30, m_y - 40);

}

void Player::MoveX(float x) {
	m_xRemainder += x;
	int move = (int)m_xRemainder;

	if (move == 0)
		return;

	m_xRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {

		if (OverlapsSolidX(sign, sign)) {
			m_velocityX = .0f;
			break;
		}

		m_x += sign;
		move -= sign;

	}
}

void Player::MoveY(float y) {
	m_yRemainder += y;
	int move = round(m_yRemainder);

	if (move == 0)
		return;

	m_yRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {
		
		if (OverlapsSolidY(sign, sign)) {
			m_velocityY = .0f;
			if (move > 0) {
				m_onFloor = true;
			}
			break;
		}
		
		m_y += sign;
		move -= sign;
	}

	// Offset the player if the stick tip is overlapping the world
	if (m_stickState == m_stickIdleState || m_stickState == m_stickHitState) {
		
		VerticalLine stickLine = { m_x + 20 * m_direction, m_y + 3, m_y + 4 };
		for (int i = 0; i < 16 && m_levelRef->OverlapsLine(stickLine); i++) {
			m_x += -m_direction;
			stickLine = { m_x + 20 * m_direction, m_y + HORIZONTAL_STICK_TIP_Y1, m_y + HORIZONTAL_STICK_TIP_Y2 };
		}
	}

}

void Player::Draw(Graphics& graphics) {

	m_bodySprite->Draw(graphics, graphics.playerBodyTexture, m_direction);
	m_stickSprite->Draw(graphics, graphics.playerStickTexture, m_direction);

	//SDL_Rect rect;
	//rect.x = m_boundingBox.X1() + m_x;
	//rect.y = m_boundingBox.Y1() + m_y;
	//rect.w = m_boundingBox.Width();
	//rect.h = m_boundingBox.Height();

	//SDL_SetRenderDrawColor(graphics.m_renderer, 0, 255, 0, 255);
	//SDL_RenderDrawRect(graphics.m_renderer, &rect);

	//int x1 = m_stickCollisionLine.X() * m_direction + m_x;
	//int y1 = m_stickCollisionLine.Start() + m_y;
	//int x2 = m_stickCollisionLine.X() * m_direction + m_x;
	//int y2 = m_stickCollisionLine.End() + m_y;

	//SDL_SetRenderDrawColor(graphics.m_renderer, 255, 0, 0, 255);
	//SDL_RenderDrawLine(graphics.m_renderer, x1, y1, x2, y2);
}

void Player::SetPosition(int x, int y) {
	m_x = x;
	m_y = y;
	m_velocityY = 0;
}

void Player::SetAnimation(Animation** target, Animation* animation) {
	*target = animation;
	(*target)->Play();
}

bool Player::IsRiding(Chunk& chunk) {
	HorizontalLine feetLine = { m_y + m_boundingBox.Y2(), m_x + m_boundingBox.X1(), m_x + m_boundingBox.X2() };

	int y = feetLine.Y();
	int x = feetLine.Start();

	// Horizontal
	while (1) {
		int valueAtFeetPoint = chunk.ValueAtPoint(x, y);
		int valueAtUnderFeetPoint = chunk.ValueAtPoint(x, y + 1);
		if (valueAtFeetPoint > 0 && valueAtUnderFeetPoint != 1) {
			return true;
		}
		if (x < feetLine.End() - 1) {
			x += std::min(feetLine.End() - 1 - x, TILE_SIZE);
			continue;
		}
		break;
	}

	return false;


}

bool Player::OverlapsSolidX(int dirX, int offset) {

	VerticalLine bodyLine = { m_x + m_boundingBox.X2() * dirX + offset, m_y + m_boundingBox.Y1(), m_y + m_boundingBox.Y2() };

	if (m_levelRef->OverlapsLine(bodyLine)) {
		return true;
	}

	if (m_stickState == m_stickIdleState || m_stickState == m_stickHitState) {
		VerticalLine stickLine = { m_x + HORIZONTAL_STICK_TIP_X * dirX + offset, m_y + 3, m_y + 4 };

		if (m_levelRef->OverlapsLine(stickLine)) {
			return true;
		}
	}

	return false;
}

bool Player::OverlapsSolidY(int dirY, int offset) {
	HorizontalLine bodyLine = { m_y + m_boundingBox.Y2() * dirY + offset, m_x + m_boundingBox.X1(), m_x + m_boundingBox.X2() };

	if (m_levelRef->OverlapsLine(bodyLine)) {
		return true;
	}

	if (dirY < 0 && m_stickState != m_stickIdleState && m_stickState != m_stickHitState) {
		HorizontalLine stickLine = { m_y + VERTICAL_STICK_TIP_Y * dirY + offset, m_x - VERTICAL_STICK_TIP_X1 * m_direction, m_x - VERTICAL_STICK_TIP_X2 + 1 * m_direction };
		if (m_levelRef->OverlapsLine(stickLine)) {
			return true;
		}
	}

	return false;
}

bool Player::HitAtPoint(int x, int y, const Vector2& direction) {
	
	for (Chunk& chunk : m_levelRef->v_chunks) {
		int ValueAtOrigin = chunk.ValueAtPoint(x + 24 * -direction.x, y + 24 * -direction.y);
		int levelValueAtStickPoint = m_levelRef->ValueAtPoint(x, y);

		int chunkValueAtStickPoint = chunk.ValueAtPoint(x, y);
		TileType* tile = chunk.TileAtPoint(x, y);

		// If I hit a solid inside the Chunk
		if (tile && tile->Solid()) {
			m_levelRef->m_tileHitFx.Reset(x, y, direction.x, direction.y);
			return tile->Hit(chunk, direction);
		}

		// If I hit the void
		if (levelValueAtStickPoint == 0 && ValueAtOrigin == 1) {
			m_levelRef->m_tileHitFx.Reset(x, y, direction.x, direction.y);
			return chunk.Slide(direction);
		}
	}

	return true;

}

void Player::InvertDirection() {
	if (m_direction == DIRECTION_LEFT) {
		m_direction = DIRECTION_RIGHT;
	}
	else {
		m_direction = DIRECTION_LEFT;
	}
}