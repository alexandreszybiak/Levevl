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
		return player->m_playerJumpState->Reset(player->m_airAcceleration);
	}

	if (player->m_velocityY >= 1.0f) {
		return player->m_playerFallState->Reset(player->m_airAcceleration);
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

	// Move left and right

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

	// Start wall slide

	VerticalLine frontLine = { player->m_x + HORIZONTAL_STICK_TIP_X * player->m_direction + player->m_direction, player->m_y + 3, player->m_y + 4 };

	if (player->m_stickState != player->m_stickIdleState && player->m_stickState != player->m_stickHitState) {

		frontLine = { player->m_x + BODY_FRONT_X * player->m_direction + player->m_direction, player->m_y + 3, player->m_y + 4 };

	}

	if (player->m_levelRef->OverlapsLine(frontLine) && inputHDir == player->m_direction) {
		return player->m_playerWallSlideState;
	}

	// Make a smaller jump

	if (input.ReleasedJump()) {
		player->m_velocityY *= 0.5;
	}

	return NULL;
}

PlayerState* PlayerJumpState::Update(Player* player) {

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -12.0f, 12.0f);

	if (player->m_velocityY >= 0) {
		return player->m_playerFallState->Reset(player->m_airAcceleration);
	}

	return NULL;
}

// Fall

void PlayerFallState::Enter(Player* player) {
	player->SetOnFloor(false);
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_jumpAnimation);
}

PlayerState* PlayerFallState::HandleInput(Player* player, Input& input) {

	// Move left & right

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

	// Start wall slide

	bool isStickWallSlide = true;

	VerticalLine frontLine = { player->m_x + HORIZONTAL_STICK_TIP_X * player->m_direction + player->m_direction, player->m_y + 3, player->m_y + 4 };

	if (player->m_stickState != player->m_stickIdleState && player->m_stickState != player->m_stickHitState) {
		frontLine = { player->m_x + BODY_FRONT_X * player->m_direction + player->m_direction, player->m_y + 3, player->m_y + 4 };
		isStickWallSlide = false;
	}

	if (player->m_levelRef->OverlapsLine(frontLine) && inputHDir == player->m_direction) {
		return player->m_playerWallSlideState->Reset(isStickWallSlide);
	}

	return NULL;
}

PlayerState* PlayerFallState::Update(Player* player) {
	if (player->OnFloor()) {
		return player->m_playerIdleState;
	}

	player->m_velocityY = std::clamp(player->m_velocityY + GRAVITY, -12.0f, 12.0f);

	return NULL;
}

// Wall Slide

void PlayerWallSlideState::Enter(Player* player) {}

PlayerState* PlayerWallSlideState::HandleInput(Player* player, Input& input) {

	int inputHDir = -input.HoldingLeft() + input.HoldingRight();

	if (player->m_direction == DIRECTION_LEFT && !input.HoldingLeft()) {
		return player->m_playerFallState->Reset(player->m_groundAcceleration);
	}
	if (player->m_direction == DIRECTION_RIGHT && !input.HoldingRight()) {
		return player->m_playerFallState->Reset(player->m_groundAcceleration);
	}

	// Wall jump

	if (input.PressedJump()) {
		player->m_velocityX = player->m_speed * 1.5f * -inputHDir;
		player->m_velocityY = player->m_jumpStrength * 0.9f;
		return player->m_playerJumpState->Reset(0.125f);
	}

	// Cancel wall slide by changing stick position
	if(m_isStickWallSlide){
		if (input.PressedUp() || input.PressedDown()) {
			return player->m_playerFallState->Reset(player->m_groundAcceleration);
		}
	}
	else {
		if (inputHDir != 0) {
			return player->m_playerFallState->Reset(player->m_groundAcceleration);
		}
	}

	return NULL;
}
PlayerState* PlayerWallSlideState::Update(Player* player) {
	if (player->OnFloor()) {
		return player->m_playerIdleState;
	}

	if (player->m_levelRef->ValueAtPoint(player->m_x + HORIZONTAL_STICK_TIP_X * player->m_direction + player->m_direction, player->m_y + 4) == 1) {
		return player->m_playerFallState->Reset(player->m_groundAcceleration);
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
		return player->m_stickAimingUpState;
	}
	else if (input.PressedDown()) {
		return player->m_stickAimingDownState;
	}

	if (input.PressedHit()) {
		return player->m_stickHitState;
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
		return player->m_stickHitDownState;
	}

	if (!input.HoldingDown()) {
		if (input.HoldingUp()) {
			return player->m_stickAimingUpState;
		}
		return player->m_stickIdleState;
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
		return player->m_stickHitUpState;
	}

	if (!input.HoldingUp()) {
		if (input.HoldingDown()) {
			return player->m_stickAimingDownState;
		}
		return player->m_stickIdleState;
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

	Vector2 hitDirection = { player->m_direction, 0 };

	bool slide = player->HitAtPoint(player->m_x + 25 * player->m_direction, player->m_y + 4, hitDirection);
	if (!slide) {
		player->m_velocityX = -3.0f * player->m_direction;
	}
}

PlayerState* StickHitState::HandleInput(Player* player, Input& input) {

	if (input.PressedHit()) {
		if (input.HoldingUp()) {
			return player->m_stickHitUpState;
		}
		else if (input.HoldingDown()) {
			return player->m_stickHitDownState;
		}
		else {
			Enter(player);
		}
	}

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.HoldingUp()) {
			return player->m_stickAimingUpState;
		}
		else if (input.HoldingDown()) {
			return player->m_stickAimingDownState;
		}
		else {
			return player->m_stickIdleState;
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
	player->HitAtPoint(player->m_x - 7 * player->m_direction, player->m_y + 32, Vector2::Down());
}

PlayerState* StickHitDownState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.HoldingDown()) {
			return player->m_stickAimingDownState;
		}
		else {
			return player->m_stickIdleState;
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
	player->HitAtPoint(player->m_x - 7 * player->m_direction, player->m_y - 32, Vector2::Up());
}

PlayerState* StickHitUpState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.HoldingUp()) {
			return player->m_stickAimingUpState;
		}
		else {
			return player->m_stickIdleState;
		}
	}

	return NULL;
}

PlayerState* StickHitUpState::Update(Player* player) {
	return NULL;
}