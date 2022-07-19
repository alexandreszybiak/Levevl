#pragma once

#define MAP_WIDTH 27
#define MAP_HEIGHT 15

struct SDL_Rect;
class Input;
class Graphics;

class Map {
public:
	static const int m_mapWidth = 27;
	static const int m_mapHeight = 15;
	Uint8 m_data[m_mapWidth * m_mapHeight];

protected:
	SDL_Rect m_destinationRect;

private:
	//

public:
	Map();
	~Map();
	virtual void Draw(Graphics& graphics);
	void Update(Input& input);
	int Edit(int mouseX, int mouseY, char value);
	virtual void SetRegion(Uint8 value, int x1, int y1, int x2, int y2);

protected:
	//

private:
	//
};

