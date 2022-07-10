#pragma once
#include "SDL.h"
#include "Map.h"

class Graphics;

//A piece of a level that can be moved around
class Chunk : public Map {
public:

private:
	int m_x;
	int m_y;
	static const int m_width = 8;
	static const int m_height = 8;
	char m_data[m_width][m_height];
	SDL_Rect m_emptyRect, m_brickRect;
	SDL_Rect m_destinationRect;
	
public:
	Chunk(int x, int y);
	~Chunk();
	void Draw(Graphics& graphics);
	void DrawMask(Graphics& graphics);
	void Move(int x, int y);
	int Edit(int mouseX, int mouseY, char value);

private:
	void Fill(char value);
};