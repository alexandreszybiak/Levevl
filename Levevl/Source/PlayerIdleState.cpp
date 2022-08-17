#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

void PlayerIdleState::Enter(Player* player) {
	std::cout << "PlayerIdleState Created" << std::endl;
	player->SetAnimation(&player->m_currentBodyAnimation, player->m_idleAnimation);
}

PlayerState* PlayerIdleState::HandleInput(Player* player, Input& input) {
	int hDir = -(input.IsKeyHeld(SDL_SCANCODE_LEFT) || input.IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) + (input.IsKeyHeld(SDL_SCANCODE_RIGHT) || input.IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));
		
	if (hDir > 0)
		player->SetDirection(DIRECTION_RIGHT);
	else if (hDir < 0)
		player->SetDirection(DIRECTION_LEFT);

	if (input.WasKeyPressed(SDL_SCANCODE_SPACE) || input.WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_B)) {
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