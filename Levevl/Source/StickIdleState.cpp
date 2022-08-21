#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerState.h"
#include "StickIdleState.h"
#include "StickHitState.h"
#include "StickAimingUpState.h"
#include "StickAimingDownState.h"

void StickIdleState::Enter(Player* player) {
	std::cout << "StickHorizontalState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickIdleAnimation);
}

PlayerState* StickIdleState::HandleInput(Player* player, Input& input) {
	if (input.WasKeyPressed(SDL_SCANCODE_UP)) {
		return new StickAimingUpState();
	}
	else if (input.WasKeyPressed(SDL_SCANCODE_DOWN)) {
		return new StickAimingDownState();
	}

	if (input.WasKeyPressed(SDL_SCANCODE_X) || input.WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y)) {
		return new StickHitState();
	}

	return NULL;
}

PlayerState* StickIdleState::Update(Player* player) {
	return NULL;
}