#pragma once
#include <vector>

class HorizontalLine;
class VerticalLine;
class Map;
class Chunk;
class Player;

class Level {
public:
	Map* worldMap;
	std::vector<Chunk> v_chunks;

	Player* player;

private:
	//

public:
	Level();
	~Level();
	Chunk* BuildChunk(int x, int y, int width, int height, char initValue);
	void DeleteChunk(int index);

	int ValueAtPoint(int x, int y);

	bool OverlapsLine(HorizontalLine& line);
	bool OverlapsLine(VerticalLine& line);

private:
	//
};