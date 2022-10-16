#pragma once
#include <vector>

class Graphics;
class Chunk;
class TileType;

class TileMap {
private:
	int m_tileSize;
	int m_width, m_height;
	std::vector<TileType*> m_data;
public:
	//
private:
	//
public:
	TileMap(int width, int height, int tileSize, TileType* initTile);
	void Draw(Graphics& graphics, int x, int y);
	void SetRegion(TileType* value, int x1, int y1, int x2, int y2);
	TileType* GetTileTypeAtPoint(int x, int y);
};

enum TileTypes {
	TILE_TYPE_NOTHING,
	TILE_TYPE_EMPTY,
	TILE_TYPE_BRICK
};

class TileType {
protected:
	static int m_tileSize;
public:
	//virtual void Hit(Chunk& chunk) = 0;
	virtual bool Solid() = 0;
	virtual void Draw(Graphics& graphics, int x, int y) = 0;

};

class NothingTile : public TileType {
	//void Hit();
	bool Solid() { return false; }
	void Draw(Graphics& graphics, int x, int y);
};

class EmptyTile : public TileType {
public:
	//void Hit();
	bool Solid() { return false; }
	void Draw(Graphics& graphics, int x, int y);
};

class BrickTile : public TileType {
public:
	//void Hit();
	bool Solid() { return true; }
	void Draw(Graphics& graphics, int x, int y);
};