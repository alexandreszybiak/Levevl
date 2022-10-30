#pragma once
#include "Entity.h"
#include "Sprite.h"

class Graphics;
class Input;

class BouncingEnemy : public Entity {
public:
	//
private:
	Sprite m_sprite;
public:
	BouncingEnemy();
	void Update(Input& input);
	void Draw(Graphics& graphics) override;

private:
	//
};