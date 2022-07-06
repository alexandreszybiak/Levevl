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
	void update();
	void render();
	void clean();



	bool running() { return isRunning; }

	static SDL_Renderer* renderer;
	static SDL_Texture* gameTexture;
	SDL_Rect viewportRect;
	static SDL_Texture* chunkTexture;
	static SDL_Texture* chunkMaskTexture;
	static SDL_Texture* worldTexture;
	static bool leftMouseButtonPressed;
	static bool rightMouseButtonPressed;
	static bool num1KeyPressed;
	static bool num2KeyPressed;
	static bool spaceKeyPressed;
	static bool leftKeyPressed;
	static bool rightKeyPressed;
	static bool upKeyPressed;
	static bool downKeyPressed;
	static bool fullscreenKeyPressed;
	static int mouseX;
	static int mouseY;

	static Input* input;

	static Map* worldMap;
	Chunk* chunks[10];
	Chunk* selectedChunk;

private:
	bool isRunning;
	int frameCount;
	SDL_Window* window;

	void Loop();
};