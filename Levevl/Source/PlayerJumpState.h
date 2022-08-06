#pragma once
#include "PlayerState.h"

class Player;
class PlayerState;

class PlayerJumpState : public PlayerState {
public:
	PlayerJumpState();
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input);
	PlayerState* Update(Player* player);
};
