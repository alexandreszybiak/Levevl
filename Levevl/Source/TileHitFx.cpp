#include "SDL.h"
#include "Utilities.h"
#include "Graphics.h"
#include "Animation.h"
#include "Sprite.h"
#include "TileHitFx.h"

TileHitFx::TileHitFx() : m_sprite(*(new Sprite(0,0,38,34,4))) {
	m_animation = new Animation(4);
	m_animation->PushFrame(0);
	m_animation->PushFrame(1);
	m_animation->PushFrame(2);
	m_animation->Play();
}

TileHitFx::~TileHitFx() {
	delete m_animation;
}

void TileHitFx::Update() {
	const KeyFrame* newFrame = m_animation->Update();

	if (newFrame) {
		m_sprite.SetFrame(newFrame->GetFrameIndex());
	}


}

void TileHitFx::Draw(Graphics& graphics) {
	m_sprite.Draw(graphics, graphics.tileHitFxTexture, DIRECTION_RIGHT, 0);
}

void TileHitFx::SetPosition(int x, int y) {
	m_sprite.SetPosition(x, y);
}