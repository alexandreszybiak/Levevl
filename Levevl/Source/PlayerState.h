#pragma once

class Input;
class Player;

class PlayerState {
public:

private:
	//
public:
	virtual void Enter(Player* player) = 0;
	virtual PlayerState* HandleInput(Player* player, Input& input) = 0;
	virtual PlayerState* Update(Player* player) = 0;
private:
	//

};