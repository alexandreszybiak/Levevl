#pragma once
#include <vector>

class Map;
class Chunk;

class Level {
public:
	Map* worldMap;
	std::vector<Chunk> v_chunks;

private:
	//

public:
	Level();
	~Level();
	Chunk* BuildChunk(int x, int y, int width, int height);
	void DeleteChunk(int index);

private:
	//
};