#pragma once

#define MAP_WIDTH 27
#define MAP_HEIGHT 15

struct SDL_Rect;
class Input;
class Graphics;

class Map {
public:
	//

protected:
	SDL_Rect m_destinationRect;

private:
	static const int m_mapWidth = 27;
	static const int m_mapHeight = 15;
	int m_data[m_mapWidth][m_mapHeight];

public:
	Map();
	~Map();
	virtual void Draw(Graphics& graphics);
	void Update(Input& input);
	int Edit(int mouseX, int mouseY, char value);

protected:
	//

private:
	//
};

