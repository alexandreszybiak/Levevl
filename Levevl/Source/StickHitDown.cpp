#include <iostream>
#include <algorithm>
#include "Input.h"
#include "Animation.h"
#include "Player.h"
#include "Sprite.h"
#include "PlayerState.h"
#include "StickAimingUpState.h"
#include "StickIdleState.h"
#include "StickHitDownState.h"
#include "StickAimingDownState.h"

void StickHitDownState::Enter(Player* player) {
	std::cout << "StickHitDownState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickHitDownAnimation);
	player->m_currentStickAnimation->Reset();
	player->m_stickSprite->SetFrame(player->m_currentStickAnimation->GetFrame()->GetFrameIndex());
	player->HitAtPoint(player->m_x - 7 * player->m_direction, player->m_y + 32, 0, 1);
}

PlayerState* StickHitDownState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.IsKeyHeld(SDL_SCANCODE_DOWN)) {
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