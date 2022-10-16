#pragma once
#include <array>
#include <vector>

class Input;
class Camera;
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

	TileHitFx& m_tileHitFx;

	// Stores all the pointers to tile types
	std::array<TileType*, 4> m_tileTypes;

	//TileType* m_tileTypes[3];


private:
	//

public:
	Level(Camera& camera);
	~Level();

	void Update(Input& input);

	// Builds a new chunk from an existing TileMap
	Chunk* BuildChunk(int x, int y, int width, int height, TileMap* tileMap);

	// Builds a new chunk from an initial value
	Chunk* BuildChunk(int x, int y, int width, int height, char initValue);

	void DeleteChunk(int index);

	int ValueAtPoint(int x, int y);

	bool OverlapsLine(HorizontalLine& line);
	bool OverlapsLine(VerticalLine& line);

private:
	//
};