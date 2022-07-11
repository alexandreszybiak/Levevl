#pragma once
#include <vector>
#include "SDL.h"
#include "Map.h"

class Graphics;

//A piece of a level that can be moved around
class Chunk : public Map {
public:

private:
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	std::vector<char> m_data;
	
	SDL_Rect m_emptyRect, m_brickRect;
	SDL_Rect m_destinationRect;
	
public:
	Chunk(int x, int y, int width, int height);
	Chunk(const Chunk& chunkCopy);
	~Chunk();
	void Draw(Graphics& graphics);
	void DrawMask(Graphics& graphics);
	void Move(int x, int y);
	int Edit(int mouseX, int mouseY, char value);
	int GetX();
	int GetY();
	bool OverlapsPoint(int x, int y);
	void SetRegion(int x1, int y1, int x2, int y2);

private:
	void Fill(char value);
};