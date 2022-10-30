#include "SDL.h"
#include "Graphics.h"
#include "Sprite.h"
#include "BouncingEnemy.h"
#include "Utilities.h"

BouncingEnemy::BouncingEnemy() : m_sprite(0, 0, 24, 24, 1) {
	
}

void BouncingEnemy::Update(Input& input) {

}

void BouncingEnemy::Draw(Graphics& graphics) {
	m_sprite.Draw(graphics, graphics.bouncingEnemyTexture);

}