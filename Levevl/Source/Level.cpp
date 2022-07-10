#include <vector>
#include "SDL.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"

Level::Level() {
	worldMap = new Map();
	v_chunks.push_back({ 24,24 });
	v_chunks.push_back({ 10 * 24,2 * 24 });
}

Level::~Level() {

}