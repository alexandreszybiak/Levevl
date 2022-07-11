#include <vector>
#include "SDL.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"

Level::Level() {
	worldMap = new Map();
	v_chunks.reserve(2);
	v_chunks.emplace_back(24,24, 8, 8 );
	v_chunks.emplace_back(10 * 24,2 * 24, 8, 8);
	int s = sizeof(v_chunks[0]);
	int s2 = sizeof(v_chunks[1]);
}

Level::~Level() {

}

Chunk* Level::BuildChunk(int x, int y, int width, int height) {
	v_chunks.push_back({ x,y,width,height });
	return &v_chunks[v_chunks.size() - 1];
}

void DeleteChunk(Chunk*) {

}