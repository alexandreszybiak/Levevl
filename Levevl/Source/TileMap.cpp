#include <iostream>
#include "math.h"
#include "SDL.h"
#include "Utilities.h"
#include "Game.h"
#include "Graphics.h"
#include "Camera.h"
#include "TileMap.h"
#include "Chunk.h"

TileMap::TileMap(int width, int height, int tileSize, TileType* initTile) : m_width(width), m_height(height), m_tileSize(tileSize) {
	m_data.reserve(width * height);
	for (int i = 0; i < width * height; i++) {
		m_data.push_back(initTile);
	}
}

void TileMap::Draw(Graphics& graphics, int x, int y) {


	int left = std::max(graphics.m_camera.m_x / m_tileSize - 1 - x / m_tileSize, 0);
	int right = std::min((graphics.m_camera.m_x + graphics.m_camera.m_width) / m_tileSize + 1 - x / m_tileSize, m_width);
	int top = std::max(graphics.m_camera.m_y / m_tileSize - y / m_tileSize, 0);
	int bottom = std::min((graphics.m_camera.m_y + graphics.m_camera.m_height) / m_tileSize + 1 - y / m_tileSize, m_height);



	for (int column = left; column < right; column++) {
		for (int row = top; row < bottom; row++) {
			SDL_Rect srcRect = { 0, 0, m_tileSize, m_tileSize };
			SDL_Rect dstRect = { 0, 0, m_tileSize, m_tileSize };

			dstRect.x = x + column * m_tileSize;
			dstRect.y = y + row * m_tileSize;
			TileType* tile = m_data[column + row * m_width];
			tile->Draw(graphics, dstRect.x, dstRect.y);
		}
	}
}

void TileMap::SetTile(TileType* value, int index) {
	m_data[index] = value;
}

void TileMap::SetTile(TileType* value, int x, int y) {
	m_data[x + y * m_width] = value;
}

void TileMap::SetRegion(TileType* value, int x1, int y1, int x2, int y2) {
	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			if (x < 0 || x >= m_width || y < 0 || y >= m_height)
				continue;
			m_data[x + y * m_width] = value;
		}
	}
}

TileType* TileMap::GetTile(int x, int y) {
	return m_data[x + y * m_width];
}

const std::vector<TileType*>& TileMap::GetData() {
	return m_data;
}

// Tile types

int TileType::m_tileSize = TILE_SIZE;

bool TileType::Hit(Chunk& chunk, const Vector2& direction) {
	std::cout << "You hit a tile" << std::endl;
	return false;
}

void NothingTile::Draw(Graphics& graphics, int x, int y) {

}

void EmptyTile::Draw(Graphics& graphics, int x, int y) {
	SDL_Rect srcRect = { 0, 0, m_tileSize, m_tileSize };
	SDL_Rect dstRect = { x, y, m_tileSize, m_tileSize };
	graphics.Draw(graphics.chunkTexture, &srcRect, &dstRect);
}

void BrickTile::Draw(Graphics& graphics, int x, int y) {
	SDL_Rect srcRect = { m_tileSize, 0, m_tileSize, m_tileSize };
	SDL_Rect dstRect = { x, y, m_tileSize, m_tileSize };
	graphics.Draw(graphics.chunkTexture, &srcRect, &dstRect);
}

bool BrickTile::Hit(Chunk& chunk, const Vector2& direction) {
	return chunk.Slide(direction);
}

// Turbo tile

void TurboTile::Draw(Graphics& graphics, int x, int y) {
	SDL_Rect srcRect = { m_tileSize * 2, 0, m_tileSize, m_tileSize };
	SDL_Rect dstRect = { x, y, m_tileSize, m_tileSize };
	graphics.Draw(graphics.chunkTexture, &srcRect, &dstRect);
}

bool TurboTile::Hit(Chunk& chunk, const Vector2& direction) {
	std::cout << "You hit a turbo tile!" << std::endl;
	return chunk.Slide(direction, true);
	return false;
}