#pragma once
#include <vector>

class Vector2;
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

	// Sets a tile by accessing its index
	void SetTile(TileType* value, int index);

	// Sets a tile by accessing its coordinates
	void SetTile(TileType* value, int x, int y);

	// Gets a tile by TileMap coordinates
	TileType* GetTile(int x, int y);

	// Get data
	const std::vector<TileType*>& GetData();

};

enum TileTypes {
	TILE_TYPE_NOTHING,
	TILE_TYPE_EMPTY,
	TILE_TYPE_BRICK,
	TILE_TYPE_TURBO,
	TILE_TYPE_SPAWNER
};

class TileType {
protected:
	static int m_tileSize;
	TileTypes m_type;
public:
	TileType(TileTypes type) : m_type(type) {}
	virtual bool Hit(Chunk& chunk, const Vector2& direction);
	virtual bool Solid() = 0;
	virtual bool Visible() = 0;
	virtual void Draw(Graphics& graphics, int x, int y) = 0;
	int GetType() { return m_type; }

};

class NothingTile : public TileType {

public:
	NothingTile() : TileType(TILE_TYPE_NOTHING) {}
	//void Hit();
	bool Solid() { return false; }
	bool Visible() { return false; }
	void Draw(Graphics& graphics, int x, int y);
};

class EmptyTile : public TileType {

public:
	EmptyTile() : TileType(TILE_TYPE_EMPTY) {}
	bool Solid() { return false; }
	bool Visible() { return true; }
	void Draw(Graphics& graphics, int x, int y);
};

class BrickTile : public TileType {

public:
	BrickTile() : TileType(TILE_TYPE_BRICK) {}
	bool Hit(Chunk& chunk, const Vector2& direction);
	bool Solid() { return true; }
	bool Visible() { return true; }
	void Draw(Graphics& graphics, int x, int y);
};

class TurboTile : public TileType {

public:
	TurboTile() : TileType(TILE_TYPE_TURBO) {}
	bool Hit(Chunk& chunk, const Vector2& direction);
	bool Solid() { return true; }
	bool Visible() { return true; }
	void Draw(Graphics& graphics, int x, int y);
};

class SpawnerTile : public TileType {
public:
	SpawnerTile() : TileType(TILE_TYPE_SPAWNER) {}
	bool Solid() { return false; }
	bool Visible() { return true; }
	void Draw(Graphics& graphics, int x, int y);
};