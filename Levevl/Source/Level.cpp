#include <vector>
#include "SDL.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"
#include "Player.h"

Level::Level() {
	worldMap = new Map();
	player = new Player(0xeeeeeeee, 0xeeeeeeee);
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