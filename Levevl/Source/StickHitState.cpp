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

void StickHitState::Enter(Player* player) {
	std::cout << "StickHitState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickHitAnimation);
	player->m_currentStickAnimation->Reset();
	player->m_stickSprite->SetFrame(player->m_currentStickAnimation->GetFrame()->GetFrameIndex());
	player->HitAtPoint(player->m_x + 25 * player->m_direction, player->m_y + 4, player->m_direction, 0);
}

PlayerState* StickHitState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.IsKeyHeld(SDL_SCANCODE_UP)) {
			return new StickAimingUpState();
		}
		else if (input.IsKeyHeld(SDL_SCANCODE_DOWN)) {
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