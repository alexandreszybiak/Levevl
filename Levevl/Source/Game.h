#pragma once
#include "SDL.h"
#include "Input.h"

#define MAX_CHUNK 10
#define TILE_SIZE 24

class Map;
class Chunk;

class Game {
public:
	Game();
	~Game();

	void handleEvents();
	void update(Input& input);
	void render();
	void clean();



	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Texture* gameTexture;
	SDL_Rect viewportRect;
	static SDL_Texture* chunkTexture;
	static SDL_Texture* chunkMaskTexture;
	static SDL_Texture* worldTexture;

	static Map* worldMap;
	Chunk* chunks[10];
	Chunk* selectedChunk;

private:
	bool isRunning;
	int frameCount;
	SDL_Window* window;

	void Loop();
};