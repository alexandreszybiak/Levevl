#include "SDL.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "Game.h"

GameObject::GameObject(int x, int y, const char* textureFilename) {
	objectTexture = TextureManager::LoadTexture(textureFilename);
	xpos = x;
	ypos = y;
	sourceRect = { 0, 0, TILE_SIZE, TILE_SIZE };
	destinationRect.w = TILE_SIZE * 3;
	destinationRect.h = TILE_SIZE * 3;
}

GameObject::~GameObject() {

}

void GameObject::Update() {
	xpos++;
	ypos++;
	destinationRect.x = xpos;
	destinationRect.y = ypos;
}

void GameObject::Render() {
	SDL_RenderCopy(Game::renderer , objectTexture, &sourceRect, &destinationRect);
}