#include <vector>
#include "SDL.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"

Level::Level() {
	//World
	worldMap = new Map();
	//chunks[0] = new Chunk(TILE_SIZE, TILE_SIZE);
	//chunks[1] = new Chunk(9 * TILE_SIZE, TILE_SIZE);


	v_chunks.push_back({ 24,24 });
	v_chunks.push_back({ 10 * 24,2 * 24 });
}

Level::~Level() {

}