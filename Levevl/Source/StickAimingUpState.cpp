#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerState.h"
#include "StickAimingUpState.h"
#include "StickIdleState.h"
#include "StickHitUpState.h"

void StickAimingUpState::Enter(Player* player) {
	std::cout << "StickAimingUpState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickIdleUpAnimation);
}

PlayerState* StickAimingUpState::HandleInput(Player* player, Input& input) {

	if (input.WasKeyPressed(SDL_SCANCODE_X) || input.WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y)) {
		return new StickHitUpState();
	}

	if (input.WasKeyReleased(SDL_SCANCODE_UP)) {
		return new StickIdleState();
	}

	return NULL;
}

PlayerState* StickAimingUpState::Update(Player* player) {
	return NULL;
}