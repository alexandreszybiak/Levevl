#pragma once
#include <array>
#include <vector>

class Input;
class Graphics;
class Camera;
class Entity;
class HorizontalLine;
class VerticalLine;
class TileMap;
class TileType;

class NothingTile;
class EmptyTile;
class BrickTile;

class Map;
class Chunk;
class Player;
class TileHitFx;

class Level {
public:
	Map* worldMap;
	std::vector<Chunk> v_chunks;

	Camera& m_camera;

	Player* player;

	TileHitFx* m_tileHitFx;

	std::array<Entity*, 20> m_entities;

	// Stores all the pointers to tile types
	std::array<TileType*, 6> m_tileTypes;

private:
	// DrawList
	int m_entitiesLastIndex;

public:
	Level(Camera& camera);
	~Level();

	void Update(Input& input);

	void DrawEntities(Graphics& graphics);

	// Builds a new chunk from an existing TileMap
	Chunk* BuildChunk(int x, int y, int width, int height, TileMap* tileMap);

	// Builds a new chunk from an initial value
	Chunk* BuildChunk(int x, int y, int width, int height, char initValue);

	// Adds an entity to the draw list
	void AddEntityToDrawList(Entity* entity);

	// Removes an entity from the draw list
	void RemoveEntityFromDrawList(Entity* entity);

	//
	bool CanAddEntity();

	// Activate tiles when entering a room
	void ActivateTiles();

	void DeleteChunk(Chunk* chunk);

	int ValueAtPoint(int x, int y);

	// Returns true if the level is solid at a given point
	bool SolidAtPoint(int x, int y);

	// Returns true if the line overlaps solids
	bool LineOverlapsSolid(HorizontalLine& line);

	// Returns true if the line overlaps solids
	bool LineOverlapsSolid(VerticalLine& line);

private:
	//
};