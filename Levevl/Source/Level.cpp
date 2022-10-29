#include <vector>
#include "SDL.h"
#include "Utilities.h"
#include "Game.h"
#include "Camera.h"
#include "TileMap.h"
#include "Map.h"
#include "Chunk.h"
#include "Level.h"
#include "Player.h"
#include "TileHitFx.h"

Level::Level(Camera& camera) : m_camera(camera), m_tileHitFx(*(new TileHitFx())) {

	worldMap = new Map();
	player = new Player(986, 175, this);
	m_camera.m_target = player;
	m_camera.m_levelRef = this;

	// Add entities to list
	m_entities.push_back(player);
	m_entities.push_back(&m_tileHitFx);

	// Init tile types
	m_tileTypes[TILE_TYPE_NOTHING] = new NothingTile();
	m_tileTypes[TILE_TYPE_EMPTY] = new EmptyTile();
	m_tileTypes[TILE_TYPE_BRICK] = new BrickTile();
	m_tileTypes[TILE_TYPE_TURBO] = new TurboTile();
	m_tileTypes[TILE_TYPE_SPAWNER] = new SpawnerTile();

}

Level::~Level() {

}

void Level::Update(Input& input) {
	// Move chunks
	for (Chunk& chunk : v_chunks) {
		chunk.Update();
	}

	for (Entity* e : m_entities){
		e->Update(input);
	}

	m_camera.Update();

	
}

Chunk* Level::BuildChunk(int x, int y, int width, int height, TileMap* tileMap) {
	//v_chunks.push_back({ x,y,width,height,initValue,this });
	v_chunks.emplace_back(x,y,width,height,tileMap,this);
	return &v_chunks[v_chunks.size() - 1];
}

Chunk* Level::BuildChunk(int x, int y, int width, int height, char initValue) {

	TileMap* newTileMap = new TileMap(width, height, TILE_SIZE, m_tileTypes[initValue]);

	v_chunks.emplace_back(x, y, width, height, newTileMap, this);
	return &v_chunks[v_chunks.size() - 1];
}

void Level::DeleteChunk(Chunk* chunk, int index) {
	delete chunk->m_tileMap;
	v_chunks.erase(v_chunks.begin() + index);
}

void Level::ActivateTiles() {
	for (Chunk& chunk : v_chunks) {
		chunk.ActivateTiles();
	}
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

bool Level::SolidAtPoint(int x, int y) {
	for (Chunk& chunk : v_chunks) {
		TileType* tile = chunk.TileAtPoint(x, y);
		if (tile == nullptr)
			continue;
		if (tile->Solid())
			return true;
		else
			return false;

	}
	return true;
}

bool Level::LineOverlapsSolid(VerticalLine& line) {
	int x = line.X();
	int y = line.Start();

	// Horizontal
	while (1) {
		if (SolidAtPoint(x, y))
			return true;
		if (y < line.End() - 1) {
			y += std::min(line.End() - 1 - y, TILE_SIZE);
			continue;
		}
		break;
	}

	return false;
}

bool Level::LineOverlapsSolid(HorizontalLine& line) {
	int y = line.Y();
	int x = line.Start();

	// Horizontal
	while (1) {
		if (SolidAtPoint(x, y))
			return true;
		if (x < line.End() - 1) {
			x += std::min(line.End() - 1 - x, TILE_SIZE);
			continue;
		}
		break;
	}

	return false;
}