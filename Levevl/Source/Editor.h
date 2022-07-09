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
private:
	//
public:
	//
private:
	char m_brushValue;
	Chunk* m_selectedChunk;
	Level* m_level_ref;
};