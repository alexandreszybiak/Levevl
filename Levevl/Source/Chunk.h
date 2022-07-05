#pragma once
#include "SDL.h"

class Chunk {
public:

private:
	static const int m_width = 5;
	static const int m_height = 5;
	SDL_Texture* m_texture;
	SDL_Texture* m_maskTexture;
	SDL_Rect m_emptyRect, m_brickRect, m_destinationRect;
	int m_x;
	int m_y;
	char m_data[m_width][m_height];

public:
	Chunk(int x, int y);
	~Chunk();
	void Fill(char value);
	void DrawMap();
	void DrawMask();
	void Move(int x, int y);
	int Edit(int mouseX, int mouseY, char value);
};