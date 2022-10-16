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

	TileMap* m_testTileMap;

	// Stores all the pointers to tile types
	std::array<TileType*, 3> m_tileTypes;

	//TileType* m_tileTypes[3];


private:
	//

public:
	Level(Camera& camera);
	~Level();

	void Update(Input& input);

	Chunk* BuildChunk(int x, int y, int width, int height, char initValue);
	void DeleteChunk(int index);

	int ValueAtPoint(int x, int y);

	bool OverlapsLine(HorizontalLine& line);
	bool OverlapsLine(VerticalLine& line);

private:
	//
};