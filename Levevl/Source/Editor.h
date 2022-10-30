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
	int m_cursorX, m_cursorY;
	int m_selectionOriginX, m_selectionOriginY;
	char m_brushValue;
	BrushMode m_brushMode;
	Chunk* m_selectedChunk;
	Level* m_level_ref;
	SDL_Rect m_selection;
	bool m_drawGuides;
	
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