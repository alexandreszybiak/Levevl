#pragma once
#include <vector>
#include "Map.h"

class Input;
class Map;
class Chunk;
class Level;

class Editor {
public:
	Editor();
	Editor(Level* level);
	~Editor();
	void Update(Input& input);
	void Draw(Graphics& graphics);
private:
	void Save();
	void Load();
public:
	//
private:
	char m_brushValue;
	Chunk* m_selectedChunk;
	int m_selectedChunkIndex;
	Level* m_level_ref;
	SDL_Rect m_selection;
};