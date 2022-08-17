#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "Graphics.h"
#include "Input.h"
#include "Sprite.h"
#include "Animation.h"

Player::Player(int x, int y, Level* level) : Entity(x, y), m_direction(DIRECTION_RIGHT), m_currentBodyAnimation(nullptr), m_onFloor(false), m_stickCollisionLine({ 25,4,5 }), m_boundingBox({ -12,-16,12,16 }), m_xRemainder(.0f), m_yRemainder(.0f), m_levelRef(level) {

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

	m_bodySprite = new Sprite(-30, -32, this, 60, 48, 5);

	m_stickSocket = new Entity(0, 0, 0, 0, this);

	m_stickSprite = new Sprite(-30, -32, m_stickSocket, 60, 48, 5);

}

void Player::Update(Input& input, Level* level) {

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

	MoveX(m_velocityX);
	MoveY(m_velocityY);

}

void Player::MoveX(float x) {
	m_xRemainder += x;
	int move = (int)m_xRemainder;

	if (move == 0)
		return;

	m_xRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {
		VerticalLine line = { m_x + m_boundingBox.X2() * sign + sign, m_y + m_boundingBox.Y1(), m_y + m_boundingBox.Y2()};
		if (!m_levelRef->OverlapsLine(line)) {
			m_x += sign;
			move -= sign;
		}
		else {
			break;
		}
	}
}

void Player::MoveY(float y) {
	m_yRemainder += y;
	int move = (int)m_yRemainder;

	if (move == 0)
		return;

	m_yRemainder -= move;
	int sign = Sign(move);

	while (move != 0) {
		HorizontalLine line = { m_y + m_boundingBox.Y2() * sign + sign, m_x + m_boundingBox.X1(), m_x + m_boundingBox.X2() };
		if (!m_levelRef->OverlapsLine(line)) {
			m_y += sign;
			move -= sign;
		}
		else {
			if (move > 0) {
				m_velocityY = .0f;
				m_onFloor = true;
			}
			break;
		}
	}
}



void Player::PostUpdate() {

	// CLEAN - make it a child list
	m_bodySprite->Update();
	m_stickSocket->Update();
	m_stickSprite->Update();
}

void Player::Draw(Graphics& graphics) {

	//m_bodySprite->Draw(graphics, graphics.playerBodyTexture, m_direction);
	//m_stickSprite->Draw(graphics, graphics.playerStickTexture, m_direction);

	SDL_Rect rect;
	rect.x = m_boundingBox.X1() + m_x;
	rect.y = m_boundingBox.Y1() + m_y;
	rect.w = m_boundingBox.Width();
	rect.h = m_boundingBox.Height();

	SDL_SetRenderDrawColor(graphics.m_renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(graphics.m_renderer, &rect);

	int x1 = m_stickCollisionLine.X() * m_direction + m_x;
	int y1 = m_stickCollisionLine.Start() + m_y;
	int x2 = m_stickCollisionLine.X() * m_direction + m_x;
	int y2 = m_stickCollisionLine.End() + m_y;

	SDL_SetRenderDrawColor(graphics.m_renderer, 255, 0, 0, 255);
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

void Player::SetState(PlayerState* state) {
	m_bodyState = state;
	m_bodyState->Enter(this);
}