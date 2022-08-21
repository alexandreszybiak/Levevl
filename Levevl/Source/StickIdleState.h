#pragma once
#include "PlayerState.h"

class Player;

class StickIdleState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};