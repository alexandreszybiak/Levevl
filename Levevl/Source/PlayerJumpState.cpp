#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Player.h"
#include "PlayerState.h"
#include "PlayerIdleState.h"
#include "PlayerJumpState.h"

PlayerJumpState::PlayerJumpState() {
	std::cout << "PlayerJumpState Created" << std::endl;
}

void PlayerJumpState::Enter(Player* player) {
	player->PlayAnimation(&player->m_jumpAnimation);
}

PlayerState* PlayerJumpState::HandleInput(Player* player, Input& input) {
	int hDir = -(input.IsKeyHeld(SDL_SCANCODE_LEFT) || input.IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) + (input.IsKeyHeld(SDL_SCANCODE_RIGHT) || input.IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT));

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