#pragma once
#include <vector>
#include "Map.h"

class Input;
class Map;
class Chunk;
class Level;



class Editor {
public:
	enum BrushMode {
		BRUSH_MODE_ERASE,
		BRUSH_MODE_DRAW
	};
	
private:
	int m_x, m_y;
	char m_brushValue;
	BrushMode m_brushMode;
	Chunk* m_selectedChunk;
	int m_selectedChunkIndex;
	Level* m_level_ref;
	SDL_Rect m_selection;
	
public:
	Editor();
	Editor(Level* level);
	~Editor();
	void Update(Input& input);
	void Draw(Graphics& graphics);

private:
	void Save();
	void Load();
};