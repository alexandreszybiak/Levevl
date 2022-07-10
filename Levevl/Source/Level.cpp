#include <vector>
#include "SDL.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"

Level::Level() {
	worldMap = new Map();
	v_chunks.push_back({ 24,24, 8, 8 });
	v_chunks.push_back({ 10 * 24,2 * 24, 8, 8 });
}

Level::~Level() {

}

Chunk* Level::BuildChunk(int x, int y, int width, int height) {
	v_chunks.push_back({ x,y,width,height });
	return &v_chunks[v_chunks.size() - 1];
}

void DeleteChunk(Chunk*) {

}