#include <fstream>
#include "SDL.h"
#include "Graphics.h"
#include "Map.h"
#include "Game.h"
#include "math.h"

Map::Map(): map() {
	brickRect = { 0,0,TILE_SIZE,TILE_SIZE };
	destinationRect = { 0,0,TILE_SIZE,TILE_SIZE };

	int defaultLevelData[MAP_WIDTH][MAP_HEIGHT] = { 0 };

	//LoadMap();
}

Map::~Map() {

}

void Map::Update(Input& input) {
	if (0) {
		map[int(floor(input.GetMouseX() / TILE_SIZE))][int(floor(input.GetMouseY() / TILE_SIZE))] = 1;
		//Save();
	}
	else if (0) {
		brushValue = 1;
	}
	else if (0) {
		brushValue = 2;
	}
	else if (0) {
		//LoadMap();
	}
}

void Map::LoadMap() {
	char tile;
	std::fstream in_level;
	in_level.open("level.txt");
	if (!in_level.is_open()) return;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int column = 0; column < MAP_WIDTH; column++) {
			tile = in_level.get();
			map[row][column] = atoi(&tile);
			in_level.ignore();
		}
	}
}

void Map::DrawMap(Graphics& graphics) {
	for (int column = 0; column < MAP_WIDTH; column++) {
		for (int row = 0; row < MAP_HEIGHT; row++) {
			SDL_Rect rect = { 0,0,TILE_SIZE,TILE_SIZE };
			// Look at neighbours horizontal
			if (column + 1 < MAP_WIDTH && map[column + 1][row] || column + 1 == MAP_WIDTH)
				rect.x += TILE_SIZE;
			if (column > 0 && map[column - 1][row] || column == 0)
				rect.x += TILE_SIZE * 2;
			// Look at neighbours vertical
			if (row + 1 < MAP_HEIGHT && map[column][row + 1] || row + 1 == MAP_HEIGHT)
				rect.y += TILE_SIZE;
			if (row > 0 && map[column][row - 1] || row == 0)
				rect.y += TILE_SIZE * 2;
			destinationRect.x = column * TILE_SIZE;
			destinationRect.y = row * TILE_SIZE;
			if (map[column][row]) {
				graphics.Draw(graphics.worldTexture, rect, destinationRect);
			}
		}
	}
}

void Map::Save() {
	std::ofstream myFile("level.txt");
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int column = 0; column < MAP_WIDTH; column++) {
			myFile << map[row][column];
			if (column == MAP_WIDTH - 1) continue;
			myFile << ",";
		}
		myFile << std::endl;
	}
	myFile.close();
}

int Map::Edit(int mouseX, int mouseY, char value) {
	int tile_x = floor(float(mouseX - 0) / TILE_SIZE);
	int tile_y = floor(float(mouseY - 0) / TILE_SIZE);
	if (tile_x < 0 || tile_x >= MAP_WIDTH || tile_y < 0 || tile_y >= MAP_HEIGHT)
		return 0;
	map[tile_x][tile_y] = value;
	return 1;
}