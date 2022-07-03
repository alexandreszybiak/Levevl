#include <fstream>
#include "SDL.h"
#include "TextureManager.h"
#include "Map.h"
#include "Game.h"
#include "math.h"

Map::Map() {
	brick = TextureManager::LoadTexture("Assets/brick.png");
	emptyRect = { 0,0,TILE_SIZE,TILE_SIZE };
	brickRect = { TILE_SIZE,0,TILE_SIZE,TILE_SIZE };
	destinationRect = { 0,0,72,72 };

	int defaultLevelData[MAP_HEIGHT][MAP_WIDTH] = { 0 };

	LoadMap();
}

Map::~Map() {

}

void Map::Update() {
	if (Game::leftMouseButtonPressed) {
		map[int(floor(Game::mouseY / TILE_RENDER_SIZE))][int(floor(Game::mouseX / TILE_RENDER_SIZE))] = brushValue;
		Save();
	}
	else if (Game::num1KeyPressed) {
		brushValue = 1;
	}
	else if (Game::num2KeyPressed) {
		brushValue = 2;
	}
	else if (Game::spaceKeyPressed) {
		LoadMap();
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

void Map::DrawMap() {
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int column = 0; column < MAP_WIDTH; column++) {
			destinationRect.y = row * TILE_SIZE * 3;
			destinationRect.x = column * TILE_SIZE * 3;
			if (map[row][column] == 1) {
				TextureManager::Draw(brick, emptyRect, destinationRect);
			}
			else if (map[row][column] == 2) {
				TextureManager::Draw(brick, brickRect, destinationRect);
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