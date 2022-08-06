#pragma once
#include "PlayerState.h"

class Player;

class PlayerIdleState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input);
	PlayerState* Update(Player* player);
};