#include "SDL.h"
#include "Game.h"
#include "Utilities.h"
#include "Graphics.h"
#include "Animation.h"
#include "Sprite.h"
#include "TileHitFx.h"

TileHitFx::TileHitFx() : m_sprite(*(new Sprite(0, 0, 14, 34, 3, 14, 16))) {
	m_animation = new Animation(2, false);
	m_animation->PushFrame(0);
	m_animation->PushFrame(1);
	//m_animation->PushFrame(2);
	m_animation->Play();

	m_sprite.SetVisible(false);

	m_sprite.SetAngle(270.0f);
}

TileHitFx::~TileHitFx() {
	delete m_animation;
}

void TileHitFx::Update() {
	const KeyFrame* newFrame = m_animation->Update();

	if (newFrame) {
		m_sprite.SetFrame(newFrame->GetFrameIndex());
	}

	if (m_animation->Playing() == false) {
		m_sprite.SetVisible(false);
	}


}

void TileHitFx::Draw(Graphics& graphics) {
	m_sprite.Draw(graphics, graphics.tileHitFxTexture, DIRECTION_RIGHT);
}

void TileHitFx::SetPosition(int x, int y) {
	m_sprite.SetPosition(x, y);
}

void TileHitFx::Reset(int x, int y, int dirX, int dirY) {
	int newX, newY;

	if (dirX > 0) {
		newX = x / TILE_SIZE * TILE_SIZE;
		newY = y / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
	}
	else if (dirX < 0) {
		newX = x / TILE_SIZE * TILE_SIZE + TILE_SIZE;
		newY = y / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
	}
	else if (dirY > 0) {
		newX = x / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
		newY = y / TILE_SIZE * TILE_SIZE;
	}
	else {
		newX = x / TILE_SIZE * TILE_SIZE + TILE_SIZE / 2;
		newY = y / TILE_SIZE * TILE_SIZE + TILE_SIZE;
	}

	m_sprite.SetPosition(newX, newY);
	m_sprite.SetVisible(true);
	const KeyFrame* newFrame = m_animation->Reset();
	m_sprite.SetFrame(newFrame->GetFrameIndex());

	double newAngle;

	if (dirX == 0 && dirY == 1)
		newAngle = 90.0f;
	else if (dirX == 0 && dirY == -1)
		newAngle = 270.0f;
	else if (dirX == 1 && dirY == 0)
		newAngle = 0.0f;
	else
		newAngle = 180.0f;

	m_sprite.SetAngle(newAngle);
}