#include <fstream>
#include "SDL.h"
#include "Graphics.h"
#include "Map.h"
#include "Game.h"
#include "math.h"

Map::Map() : m_data(), m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE }) {}

Map::~Map() {}

void Map::Update(Input& input) {}

void Map::Draw(Graphics& graphics) {
	for (int column = 0; column < MAP_WIDTH; column++) {
		for (int row = 0; row < MAP_HEIGHT; row++) {
			SDL_Rect rect = { 0,0,TILE_SIZE,TILE_SIZE };

			// Look at neighbours horizontal
			if (column + 1 < MAP_WIDTH && m_data[column + 1][row] || column + 1 == MAP_WIDTH)
				rect.x += TILE_SIZE;
			if (column > 0 && m_data[column - 1][row] || column == 0)
				rect.x += TILE_SIZE * 2;

			// Look at neighbours vertical
			if (row + 1 < MAP_HEIGHT && m_data[column][row + 1] || row + 1 == MAP_HEIGHT)
				rect.y += TILE_SIZE;
			if (row > 0 && m_data[column][row - 1] || row == 0)
				rect.y += TILE_SIZE * 2;

			m_destinationRect.x = column * TILE_SIZE;
			m_destinationRect.y = row * TILE_SIZE;
			if (m_data[column][row]) {
				graphics.Draw(graphics.worldTexture, rect, m_destinationRect);
			}
		}
	}
}



int Map::Edit(int mouseX, int mouseY, char value) {
	int tile_x = floor(float(mouseX - 0) / TILE_SIZE);
	int tile_y = floor(float(mouseY - 0) / TILE_SIZE);
	if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT)
		return 0;
	m_data[tile_x][tile_y] = value;
	return 1;
}