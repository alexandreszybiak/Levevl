#pragma once
#include <vector>
#include "SDL.h"
#include "Utilities.h"

class HorizontalLine;
class VerticalLine;
class Graphics;
class TileMap;
class TileType;
class Level;
class TileHitFx;

enum ChunkMovementType {
	CHUNK_MOVE_TYPE_EASE,
	CHUNK_MOVE_TYPE_LINEAR
};

//A piece of a level that can be moved around
class Chunk {
public:
	
	// A TileMap object to store the data
	TileMap* m_tileMap;

	TileHitFx* m_tileHitFx;

private:
	// Used to compute collisions with entities
	int m_x, m_y;

	// 
	float m_xRemainder, m_yRemainder;

	// Used to render the chunk only, can be used to add effect like chunk bump
	int m_renderX, m_renderY;

	// The grid bound position the chunk is reaching for
	int m_targetX, m_targetY;

	// Velocity
	float m_velocityX, m_velocityY;

	// Movement type
	ChunkMovementType m_chunkMovementType;

	// Turbo direction
	Vector2 m_turboDirection;

	//
	int m_width, m_height;

	//std::vector<char> m_data;
	
	SDL_Rect m_emptyRect, m_brickRect;
	SDL_Rect m_destinationRect;

	Level* m_levelRef;
	
public:
	Chunk(int x, int y, int width, int height, TileMap* tileMap, Level* levelRef);
	~Chunk();
	void Update();
	void Draw(Graphics& graphics);
	void DrawMask(Graphics& graphics);

	// Moves and updates Actors that this Chunk is colliding with
	void Move(float x, float y);
	
	int GetX();
	int GetY();
	int GetWidth();
	std::vector<char>* GetData();

	// Slide changes the target grid position of a chunk which inits a movement, return false if the chunk cannot move
	bool Slide(const Vector2& direction, bool turbo = false);

	// Checks if this chunk can move, this function can be called recursively
	bool CanSlide(const Vector2& direction, std::vector<Chunk*>& otherChunks, std::vector<Chunk*>& freeChunks);

	// Checks if this chunk overlaps the given 2d point and return the value at point
	int OverlapsPoint(int x, int y);

	// Checks if this chunk overlaps the given chunk, with
	bool OverlapsChunk(Chunk* otherChunk, int offsetX, int offsetY);

	// Checks if this chunk overlpas the world walls
	bool OverlapsWalls(int offsetX, int offsetY);

	// Sets a rectangular region of the chunk data
	void SetRegion(char value, int x1, int y1, int x2, int y2);

	// Return the value found at the given point on the chunk's tilemap data
	int ValueAtPoint(int x, int y);

	// Return the TileType at a given point in the world
	TileType* TileAtPoint(int x, int y);

private:
	//
};