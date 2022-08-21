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

void StickHitUpState::Enter(Player* player) {
	std::cout << "StickHitUpState Created" << std::endl;
	player->SetAnimation(&player->m_currentStickAnimation, player->m_stickHitUpAnimation);
	player->m_currentStickAnimation->Reset();
	player->m_stickSprite->SetFrame(player->m_currentStickAnimation->GetFrame()->GetFrameIndex());
	player->HitAtPoint(player->m_x - 7 * player->m_direction, player->m_y - 32, 0, -1);
}

PlayerState* StickHitUpState::HandleInput(Player* player, Input& input) {

	if (!player->m_currentStickAnimation->Playing()) {
		if (input.IsKeyHeld(SDL_SCANCODE_UP)) {
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