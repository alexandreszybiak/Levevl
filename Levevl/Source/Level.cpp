#include <iostream>
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

Level::Level(Camera& camera) : m_camera(camera), m_tileHitFx(*(new TileHitFx())), m_drawListLastIndex(0) {

	worldMap = new Map();
	player = new Player(986, 175, this);
	Player* player2 = new Player(986, 175, this);
	m_camera.m_target = player;
	m_camera.m_levelRef = this;

	// Add entities to list
	m_entities.push_back(player);
	m_entities.push_back(&m_tileHitFx);

	// Init draw list
	m_drawList.fill(nullptr);
	AddEntityToDrawList(player);
	AddEntityToDrawList(&m_tileHitFx);

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

void Level::DrawEntities(Graphics& graphics) {
	for (Entity* entity : m_drawList) {
		if (entity == nullptr)
			break;
		entity->Draw(graphics);
	}
}

void Level::AddEntityToDrawList(Entity* entity) {
	if (m_drawListLastIndex == m_drawList.size()) {
		std::cout << "Can't add to draw list. No more free slots." << std::endl;
		return;
	}
	m_drawList[m_drawListLastIndex] = entity;
	entity->m_drawIndex = m_drawListLastIndex;
	m_drawListLastIndex++;
}

void Level::RemoveEntityFromDrawList(Entity* entity) {
	int index = entity->m_drawIndex;
	if (index == -1)
		return;
	entity->m_drawIndex = -1;

	size_t size = sizeof(void*) * (m_drawListLastIndex - 1 - index);
	void* dst = &m_drawList + index;
	void* src = &m_drawList[index + 1];

	std::memcpy(dst, src, size);

	m_drawList[m_drawListLastIndex - 1] = nullptr;
	m_drawListLastIndex--;

	int drawIndex = 0;
	for (Entity* e : m_drawList) {
		e->m_drawIndex = drawIndex++;
	}
}

Chunk* Level::BuildChunk(int x, int y, int width, int height, TileMap* tileMap) {
	//v_chunks.push_back({ x,y,width,height,initValue,this });
	v_chunks.emplace_back(x,y,width,height,tileMap,this);

	int lastIndex = v_chunks.size() - 1;
	Chunk* newChunk = &v_chunks[lastIndex];
	newChunk->m_index = lastIndex;
	return newChunk;
}

Chunk* Level::BuildChunk(int x, int y, int width, int height, char initValue) {

	TileMap* newTileMap = new TileMap(width, height, TILE_SIZE, m_tileTypes[initValue]);

	v_chunks.emplace_back(x, y, width, height, newTileMap, this);

	int lastIndex = v_chunks.size() - 1;
	Chunk* newChunk = &v_chunks[lastIndex];
	newChunk->m_index = lastIndex;
	return newChunk;
}

void Level::DeleteChunk(Chunk* chunk) {
	delete chunk->m_tileMap;
	v_chunks.erase(v_chunks.begin() + chunk->m_index);
	int i = 0;
	for (Chunk& chunk : v_chunks) {
		chunk.m_index = i;
		i++;
	}
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