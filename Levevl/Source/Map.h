#pragma once

#define MAP_WIDTH 27
#define MAP_HEIGHT 15

struct SDL_Rect;
struct SDL_Texture;
class Input;

class Map {
public:
	Map();
	~Map();
	void LoadMap();
	void DrawMap();
	void Update(Input& input);
	void Save();
	int Edit(int mouseX, int mouseY, char value);

private:
	SDL_Rect brickRect, destinationRect;
	SDL_Texture* brick;

	Uint8 brushValue = 1;
	int map[MAP_WIDTH][MAP_HEIGHT];
};

