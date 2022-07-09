#pragma once
#include <vector>
#include "SDL.h"
#include "Input.h"

#define MAX_CHUNK 10
#define TILE_SIZE 24

class Graphics;
class Map;
class Chunk;
class Level;

class Game {
public:
	Game();
	~Game();

	void update(Input& input);
	void render(Graphics& graphics);

	bool running() { return isRunning; }

	/*static Map* worldMap;
	Chunk* chunks[10];
	Chunk* selectedChunk;*/

	Level* level;
	

private:
	bool isRunning;
	int frameCount;

	void Loop();
};