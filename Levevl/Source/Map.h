#pragma once

struct SDL_Rect;
class Input;
class Graphics;

class Map {
public:
	static const int m_mapWidth = 82;
	static const int m_mapHeight = 45;
	Uint8 m_data[m_mapWidth * m_mapHeight];

protected:
	SDL_Rect m_destinationRect;

private:
	//

public:
	Map();
	~Map();
	virtual void Draw(Graphics& graphics);
	void DrawBackground(Graphics& graphics);
	void Update(Input& input);
	int Edit(int mouseX, int mouseY, char value);
	virtual void SetRegion(Uint8 value, int x1, int y1, int x2, int y2);
	virtual int OverlapsPoint(int x, int y);

protected:
	//

private:
	//
};

