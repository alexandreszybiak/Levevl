#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerState.h"
#include "StickAimingDownState.h"
#include "StickIdleState.h"
#include "StickHitDownState.h"

void StickAimingDownState::Enter(Player* player) {
	std::cout << "StickAimingDownState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickIdleDownAnimation);
}

PlayerState* StickAimingDownState::HandleInput(Player* player, Input& input) {

	if (input.WasKeyPressed(SDL_SCANCODE_X) || input.WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y)) {
		return new StickHitDownState();
	}

	if (input.WasKeyReleased(SDL_SCANCODE_DOWN)) {
		return new StickIdleState();
	}

	return NULL;
}

PlayerState* StickAimingDownState::Update(Player* player) {
	return NULL;
}