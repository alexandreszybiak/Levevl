#include "Game.h"

class ISwitchable {
public :
	virtual void Activate() {}
};

class IBreakable {
public:
	virtual void Break() {}
};

class Switch {
	
public:
	ISwitchable* client;
	Switch() : client(nullptr) {}
	void Toggle() {
		client->Activate();
	}
};

class Door : public ISwitchable, public IBreakable {
public:
	void Activate() override {
		bool activated = true;
	}
	void Break() override {
		bool broken = true;
	}
};

int main(int argc, char* argv[]) {
	Door door;
	Switch s;
	s.client = &door;
	s.Toggle();

	IBreakable* breakable = &door;
	breakable->Break();


	Game game;
	return 0;
}