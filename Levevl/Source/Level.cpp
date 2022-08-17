#include <vector>
#include "SDL.h"
#include "Utilities.h"
#include "Game.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"
#include "Player.h"

Level::Level() {
	worldMap = new Map();
	player = new Player(0xeeeeeeee, 0xeeeeeeee, this);
}

Level::~Level() {

}

Chunk* Level::BuildChunk(int x, int y, int width, int height, char initValue) {
	//v_chunks.push_back({ x,y,width,height,initValue,this });
	v_chunks.emplace_back(x,y,width,height,initValue,this);
	return &v_chunks[v_chunks.size() - 1];
}

void Level::DeleteChunk(int index) {
	v_chunks.erase(v_chunks.begin() + index);
}

int Level::ValueAtPoint(int x, int y) {
	for (Chunk& chunk : v_chunks) {
		int value = chunk.ValueAtPoint(x, y);
		if (value == 0)
			continue;
		return value;
	}
	return 0;
}

bool Level::OverlapsLine(VerticalLine& line) {
	int x = line.X();
	int y = line.Start();

	// Horizontal
	while (1) {
		int valueAtPoint = ValueAtPoint(x, y);
		if (valueAtPoint != 1) {
			return true;
		}
		if (y < line.End() - 1) {
			y += std::min(line.End() - 1 - y, TILE_SIZE);
			continue;
		}
		break;
	}

	return false;
}

bool Level::OverlapsLine(HorizontalLine& line) {
	int y = line.Y();
	int x = line.Start();

	// Horizontal
	while (1) {
		int valueAtPoint = ValueAtPoint(x, y);
		if (valueAtPoint != 1) {
			return true;
		}
		if (x < line.End() - 1) {
			x += std::min(line.End() - 1 - x, TILE_SIZE);
			continue;
		}
		break;
	}

	return false;
}