#pragma once

#define MAP_WIDTH 20
#define MAP_HEIGHT 20
#define TILE_RENDER_SIZE 72

class Map {
public:
	Map();
	~Map();
	void LoadMap();
	void DrawMap();
	void Update();
	void Save();

private:
	SDL_Rect emptyRect, brickRect, destinationRect;
	SDL_Texture* brick;

	Uint8 brushValue = 1;
	int map[MAP_HEIGHT][MAP_WIDTH];
};

