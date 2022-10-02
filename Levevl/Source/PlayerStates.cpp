#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Level.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerStates.h"

// Body Idle

void PlayerIdleState::Enter(Player* player) {
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_idleAnimation);
}

PlayerState* PlayerIdleState::HandleInput(Player* player, Input& input) {
	int inputHDir = -input.HoldingLeft() + input.HoldingRight();

	if (inputHDir == 0) {
		if (abs(player->m_velocityX) > 0.0f) {
			player->m_velocityX -= Sign(player->m_velocityX) * player->m_groundDeceleration;
			if (abs(player->m_velocityX) < player->m_groundDeceleration) {
				player->m_velocityX = 0.0f;
			}
		}
	}
	else if (inputHDir > 0) {
		player->SetDirection(DIRECTION_RIGHT);
		if (player->m_velocityX < player->m_speed) {
			player->m_velocityX += player->m_groundAcceleration;
		}
	}
	else {
		player->SetDirection(DIRECTION_LEFT);
		if (player->m_velocityX > -player->m_speed) {
			player->m_velocityX -= player->m_groundAcceleration;
		}
	}

	// Decelerate to normal max run speed
	if (player->m_velocityX > player->m_speed) {
		player->m_velocityX -= player->m_groundAcceleration;
	}
	else if (player->m_velocityX < -player->m_speed) {
		player->m_velocityX += player->m_groundAcceleration;
	}

	//Jump
	if (input.PressedJump()) {
		player->m_velocityY = player->m_jumpStrength;
		player->SetOnFloor(false);
		return new PlayerJumpState(player->m_airAcceleration);
	}

	if (player->m_velocityY >= 1.0f) {
		return new PlayerFallState(player->m_airAcceleration);
	}

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

void PlayerJumpState::Enter(Player* player) {
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_jumpAnimation);
}

PlayerState* PlayerJumpState::HandleInput(Player* player, Input& input) {
	int inputHDir = -input.HoldingLeft() + input.HoldingRight();

	if (inputHDir > 0) {
		player->SetDirection(DIRECTION_RIGHT);
		if (player->m_velocityX < player->m_speed) {
			player->m_velocityX = std::clamp(player->m_velocityX + m_accelerationX, -ENTITY_MAX_SPEED, player->m_speed);
		}
	}
	else if (inputHDir < 0) {
		player->SetDirection(DIRECTION_LEFT);
		if (player->m_velocityX > -player->m_speed) {
			player->m_velocityX = std::clamp(player->m_velocityX - m_accelerationX, -player->m_speed, ENTITY_MAX_SPEED);
		}
	}

	if (player->m_levelRef->ValueAtPoint(player->m_x + STICK_TIP_X * player->m_direction + player->m_direction, player->m_y + 4) != 1) {
		if (inputHDir == player->m_direction) {
			return new PlayerWallSlideState();
		}
	}

	if (input.ReleasedJump()) {
		player->m_velocityY *= 0.5;
	}

	return NULL;
}

PlayerState* PlayerJumpState::Update(Player* player) {

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -12.0f, 12.0f);

	if (player->m_velocityY >= 0) {
		return new PlayerFallState(player->m_airAcceleration);
	}

	return NULL;
}

// Fall

void PlayerFallState::Enter(Player* player) {
	player->SetOnFloor(false);
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_jumpAnimation);
}

PlayerState* PlayerFallState::HandleInput(Player* player, Input& input) {
	int inputHDir = -input.HoldingLeft() + input.HoldingRight();

	if (inputHDir > 0) {
		player->SetDirection(DIRECTION_RIGHT);
		if (player->m_velocityX < player->m_speed) {
			player->m_velocityX = std::clamp(player->m_velocityX + m_accelerationX, -ENTITY_MAX_SPEED, player->m_speed);
		}
	}
	else if (inputHDir < 0) {
		player->SetDirection(DIRECTION_LEFT);
		if (player->m_velocityX > -player->m_speed) {
			player->m_velocityX = std::clamp(player->m_velocityX - m_accelerationX, -player->m_speed, ENTITY_MAX_SPEED);
		}
	}

	if (player->m_levelRef->ValueAtPoint(player->m_x + STICK_TIP_X * player->m_direction + player->m_direction, player->m_y + 4) != 1) {
		if (inputHDir == player->m_direction) {
			return new PlayerWallSlideState();
		}
	}

	return NULL;
}

PlayerState* PlayerFallState::Update(Player* player) {
	if (player->OnFloor()) {
		return new PlayerIdleState();
	}

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -12.0f, 12.0f);

	return NULL;
}

// Wall Slide

void PlayerWallSlideState::Enter(Player* player) {

}
PlayerState* PlayerWallSlideState::HandleInput(Player* player, Input& input) {
	if (player->m_direction == DIRECTION_LEFT && !input.HoldingLeft()) {
		return new PlayerFallState(player->m_groundAcceleration);
	}
	if (player->m_direction == DIRECTION_RIGHT && !input.HoldingRight()) {
		return new PlayerFallState(player->m_groundAcceleration);
	}
	if (input.PressedJump()) {
		player->InvertDirection();
		player->m_velocityX = player->m_speed * 1.5f * player->m_direction;
		player->m_velocityY = player->m_jumpStrength * 0.9f;
		return new PlayerJumpState(0.125f);
	}

	return NULL;
}
PlayerState* PlayerWallSlideState::Update(Player* player) {
	if (player->OnFloor()) {
		return new PlayerIdleState();
	}

	if (player->m_levelRef->ValueAtPoint(player->m_x + STICK_TIP_X * player->m_direction + player->m_direction, player->m_y + 4) == 1) {
		return new PlayerFallState(player->m_groundAcceleration);
	}

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -20.0f, player->m_WallSlideSpeed);

	return NULL;
}

// Stick Idle

void StickIdleState::Enter(Player* player) {
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
	bool slide = player->HitAtPoint(player->m_x + 25 * player->m_direction, player->m_y + 4, player->m_direction, 0);
	if (!slide) {
		player->m_velocityX = -3.0f * player->m_direction;
	}
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