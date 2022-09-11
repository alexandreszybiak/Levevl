#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerStates.h"

// Body Idle

void PlayerIdleState::Enter(Player* player) {
	std::cout << "PlayerIdleState Created" << std::endl;
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_idleAnimation);
}

PlayerState* PlayerIdleState::HandleInput(Player* player, Input& input) {
	int hDir = -input.HoldingLeft() + input.HoldingRight();

	if (hDir > 0)
		player->SetDirection(DIRECTION_RIGHT);
	else if (hDir < 0)
		player->SetDirection(DIRECTION_LEFT);

	if (input.PressedJump()) {
		player->m_velocityY = -8;
		player->SetOnFloor(false);
		return new PlayerJumpState();
	}

	player->m_velocityX = hDir * player->m_speed;

	return NULL;
}

PlayerState* PlayerIdleState::Update(Player* player) {

	if (player->m_velocityX != 0) {
		if (player->GetAnimation() != player->m_runAnimation) {
			player->SetAnimation(&player->m_currentBodyAnimation, player->m_runAnimation);
		}
	}
	else {
		player->SetAnimation(&player->m_currentBodyAnimation, player->m_idleAnimation);
	}

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -12.0f, 12.0f);

	return NULL;
}

// Jump

PlayerJumpState::PlayerJumpState() {
	std::cout << "PlayerJumpState Created" << std::endl;
}

void PlayerJumpState::Enter(Player* player) {
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_jumpAnimation);
}

PlayerState* PlayerJumpState::HandleInput(Player* player, Input& input) {
	int hDir = -input.HoldingLeft() + input.HoldingRight();

	if (hDir > 0)
		player->SetDirection(DIRECTION_RIGHT);
	else if (hDir < 0)
		player->SetDirection(DIRECTION_LEFT);

	player->m_velocityX = hDir * 2.5f;

	return NULL;
}

PlayerState* PlayerJumpState::Update(Player* player) {
	if (player->OnFloor()) {
		return new PlayerIdleState();
	}

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -12.0f, 12.0f);

	return NULL;
}

// Stick Idle

void StickIdleState::Enter(Player* player) {
	std::cout << "StickHorizontalState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickIdleAnimation);
}

PlayerState* StickIdleState::HandleInput(Player* player, Input& input) {
	if (input.PressedUp()) {
		return new StickAimingUpState();
	}
	else if (input.PressedDown()) {
		return new StickAimingDownState();
	}

	if (input.PressedHit()) {
		return new StickHitState();
	}

	return NULL;
}

PlayerState* StickIdleState::Update(Player* player) {
	return NULL;
}

// Stick Aiming Down

void StickAimingDownState::Enter(Player* player) {
	std::cout << "StickAimingDownState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickIdleDownAnimation);
}

PlayerState* StickAimingDownState::HandleInput(Player* player, Input& input) {

	if (input.PressedHit()) {
		return new StickHitDownState();
	}

	if (!input.HoldingDown()) {
		if (input.HoldingUp()) {
			return new StickAimingUpState();
		}
		return new StickIdleState();
	}

	return NULL;
}

PlayerState* StickAimingDownState::Update(Player* player) {
	return NULL;
}

// Stick Aiming Up

void StickAimingUpState::Enter(Player* player) {
	std::cout << "StickAimingUpState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickIdleUpAnimation);
}

PlayerState* StickAimingUpState::HandleInput(Player* player, Input& input) {

	if (input.PressedHit()) {
		return new StickHitUpState();
	}

	if (!input.HoldingUp()) {
		if (input.HoldingDown()) {
			return new StickAimingDownState();
		}
		return new StickIdleState();
	}

	return NULL;
}

PlayerState* StickAimingUpState::Update(Player* player) {
	return NULL;
}

// Stick Hit

void StickHitState::Enter(Player* player) {
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickHitAnimation);
	player->m_currentStickAnimation->Reset();
	player->m_stickSprite->SetFrame(player->m_currentStickAnimation->GetFrame()->GetFrameIndex());
	player->HitAtPoint(player->m_x + 25 * player->m_direction, player->m_y + 4, player->m_direction, 0);
}

PlayerState* StickHitState::HandleInput(Player* player, Input& input) {

	if (input.PressedHit()) {
		if (input.HoldingUp()) {
			return new StickHitUpState();
		}
		else if (input.HoldingDown()) {
			return new StickHitDownState();
		}
		else {
			Enter(player);
		}
	}

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.HoldingUp()) {
			return new StickAimingUpState();
		}
		else if (input.HoldingDown()) {
			return new StickAimingDownState();
		}
		else {
			return new StickIdleState();
		}
	}

	return NULL;
}

PlayerState* StickHitState::Update(Player* player) {
	return NULL;
}

// Stick Hit Down

void StickHitDownState::Enter(Player* player) {
	std::cout << "StickHitDownState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickHitDownAnimation);
	player->m_currentStickAnimation->Reset();
	player->m_stickSprite->SetFrame(player->m_currentStickAnimation->GetFrame()->GetFrameIndex());
	player->HitAtPoint(player->m_x - 7 * player->m_direction, player->m_y + 32, 0, 1);
}

PlayerState* StickHitDownState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.HoldingDown()) {
			return new StickAimingDownState();
		}
		else {
			return new StickIdleState();
		}
	}

	return NULL;
}

PlayerState* StickHitDownState::Update(Player* player) {
	return NULL;
}

// Stick Hit Up

void StickHitUpState::Enter(Player* player) {
	std::cout << "StickHitUpState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickHitUpAnimation);
	player->m_currentStickAnimation->Reset();
	player->m_stickSprite->SetFrame(player->m_currentStickAnimation->GetFrame()->GetFrameIndex());
	player->HitAtPoint(player->m_x - 7 * player->m_direction, player->m_y - 32, 0, -1);
}

PlayerState* StickHitUpState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.HoldingUp()) {
			return new StickAimingUpState();
		}
		else {
			return new StickIdleState();
		}
	}

	return NULL;
}

PlayerState* StickHitUpState::Update(Player* player) {
	return NULL;
}