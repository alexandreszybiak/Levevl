#include <fstream>
#include "SDL.h"
#include "Graphics.h"
#include "Camera.h"
#include "Map.h"
#include "Game.h"
#include "math.h"

Map::Map() : m_data(), m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE }) {}

Map::~Map() {}

void Map::Update(Input& input) {}

void Map::Draw(Graphics& graphics) {
	
	int left = std::max(graphics.m_camera.m_x / TILE_SIZE, 0);
	int right = std::min((graphics.m_camera.m_x + graphics.m_camera.m_width) / TILE_SIZE + 1, m_mapWidth);
	int top = std::max(graphics.m_camera.m_y / TILE_SIZE, 0);
	int bottom = std::min((graphics.m_camera.m_y + graphics.m_camera.m_height) / TILE_SIZE + 1, m_mapHeight);
	
	for (int column = left; column < right; column++) {
		for (int row = top; row < bottom; row++) {
			SDL_Rect rect = { 0,0,TILE_SIZE,TILE_SIZE };

			// Look at neighbours horizontal
			if (column + 1 < m_mapWidth && m_data[(column + 1) + row * m_mapWidth] || column + 1 == m_mapWidth)
				rect.x += TILE_SIZE;
			if (column > 0 && m_data[(column - 1) + row * m_mapWidth] || column == 0)
				rect.x += TILE_SIZE * 2;

			// Look at neighbours vertical
			if (row + 1 < m_mapHeight && m_data[column + (row + 1) * m_mapWidth] || row + 1 == m_mapHeight)
				rect.y += TILE_SIZE;
			if (row > 0 && m_data[column + (row - 1) * m_mapWidth] || row == 0)
				rect.y += TILE_SIZE * 2;

			m_destinationRect.x = column * TILE_SIZE;
			m_destinationRect.y = row * TILE_SIZE;
			if (m_data[column + row * m_mapWidth]) {
				graphics.Draw(graphics.worldTexture, &rect, &m_destinationRect);
			}
		}
	}
}

void Map::DrawBackground(Graphics& graphics) {

	

	for (int column = 0; column < 27; column++) {
		for (int row = 0; row < 15; row++) {
			int randX = std::rand() % 104;
			int randY = std::rand() % 104;
			SDL_Rect rect = { randX,randY,TILE_SIZE,TILE_SIZE };
			m_destinationRect.x = column * TILE_SIZE;
			m_destinationRect.y = row * TILE_SIZE;
			graphics.Draw(graphics.backgroundTexture, &rect, &m_destinationRect, true);
		}
	}
}

void Map::SetRegion(Uint8 value, int x1, int y1, int x2, int y2) {
	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			if (x < 0 || x >= m_mapWidth || y < 0 || y >= m_mapHeight)
				continue;
			m_data[x + y * m_mapWidth] = value;
		}
	}
}

int Map::OverlapsPoint(int x, int y) {
	int tileX = x / TILE_SIZE;
	int tileY = y / TILE_SIZE;

	if (tileX < 0 || tileX >= m_mapWidth || tileY < 0 || tileY >= m_mapHeight)
		return 0;

	return m_data[tileX + tileY * m_mapWidth];
}