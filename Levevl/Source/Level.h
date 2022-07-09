#pragma once
#include <vector>

class Map;
class Chunk;

class Level {
public:
	Level();
	~Level();
	Map* worldMap;
	std::vector<Chunk> v_chunks;
};