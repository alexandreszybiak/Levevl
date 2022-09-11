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

// Body states

class PlayerIdleState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input);
	PlayerState* Update(Player* player);
};

class PlayerJumpState : public PlayerState {
public:
	PlayerJumpState();
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input);
	PlayerState* Update(Player* player);
};


// Stick States

class StickIdleState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};

class StickAimingDownState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};

class StickAimingUpState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};

class StickHitState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};

class StickHitDownState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};

class StickHitUpState : public PlayerState {
public:
	void Enter(Player* player) override;
	PlayerState* HandleInput(Player* player, Input& input) override;
	PlayerState* Update(Player* player) override;
};