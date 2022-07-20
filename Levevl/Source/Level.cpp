#include <vector>
#include "SDL.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"

Level::Level() {
	worldMap = new Map();
}

Level::~Level() {

}

Chunk* Level::BuildChunk(int x, int y, int width, int height, char initValue) {
	v_chunks.push_back({ x,y,width,height,initValue });
	return &v_chunks[v_chunks.size() - 1];
}

void Level::DeleteChunk(int index) {
	v_chunks.erase(v_chunks.begin() + index);
}