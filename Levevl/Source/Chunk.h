#pragma once
#include <vector>
#include "SDL.h"
#include "Map.h"

class Graphics;
class Level;

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

	Level* m_levelRef;
	
public:
	Chunk(int x, int y, int width, int height, char initValue, Level* levelRef);
	Chunk(const Chunk& chunkCopy);
	~Chunk();
	void Draw(Graphics& graphics);
	void DrawMask(Graphics& graphics);
	void Move(int x, int y);

	
	int GetX();
	int GetY();
	int GetWidth();
	std::vector<char>* GetData();

	// Checks if this chunk can move, this function can be called recursively
	bool CanMove(int x, int y, std::vector<Chunk*>& otherChunks, std::vector<Chunk*>& freeChunks);

	// Checks if this chunk overlaps the given 2d point
	bool OverlapsPoint(int x, int y);

	// Checks if this chunk overlaps the given chunk, with
	bool OverlapsChunk(Chunk* otherChunk, int offsetX, int offsetY);

	// Checks if this chunk overlpas the world walls
	bool OverlapsWalls(int offsetX, int offsetY);

	// Sets a rectangular region of the chunk data
	void SetRegion(char value, int x1, int y1, int x2, int y2);

private:
	//
};