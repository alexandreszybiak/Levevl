#include <iostream>
#include "SDL.h"
#include "Input.h"
#include "Editor.h"
#include "Map"
#include "Chunk.h"
#include "Level.h"

Editor::Editor(Level* level) : m_brushValue(1) {
	m_level_ref = level;
	//std::cout << "chunks address is : " << &chunks << std::endl;
	m_selectedChunk = &m_level_ref->v_chunks[0];
	//std::cout << "m_selectedChunk points to this address : " << m_selectedChunk << std::endl;
}

Editor::~Editor() {

}

void Editor::Update(Input& input) {
	if (input.WasKeyPressed(SDL_SCANCODE_LEFT)) {
		m_selectedChunk->Move(-1, 0);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_RIGHT)) {
		m_selectedChunk->Move(1, 0);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_UP)) {
		m_selectedChunk->Move(0, -1);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_DOWN)) {
		m_selectedChunk->Move(0, 1);
	}
	if (input.WasMouseButtonPressed(SDL_BUTTON_LEFT)) {
		if (!m_selectedChunk->Edit(input.GetMouseX(), input.GetMouseY(), 1)) {
			bool b_hasFoundChunk = false;
			for (Chunk& chunk : m_level_ref->v_chunks) {
				if (chunk.Edit(input.GetMouseX(), input.GetMouseY(), 1)) {
					m_selectedChunk = &chunk;
					b_hasFoundChunk = true;
					break;
				}
			}
			if (!b_hasFoundChunk) {
				m_level_ref->worldMap->Edit(input.GetMouseX(), input.GetMouseY(), 1);
				/*for (int i = 0; i < max_chunk; i++) {
					if (!chunks[i]) {
						selectedchunk = chunks[i] = new chunk(int(floor(mousex / tile_size) * tile_size), int(floor(mousey / tile_size) * tile_size));
						break;
					}
				}*/
			}
		}
	}
	if (input.WasMouseButtonPressed(SDL_BUTTON_RIGHT)) {
		if (!m_selectedChunk->Edit(input.GetMouseX(), input.GetMouseY(), 0)) {
			for (Chunk& chunk : m_level_ref->v_chunks) {
				if (chunk.Edit(input.GetMouseX(), input.GetMouseY(), 0)) {
					m_selectedChunk = &chunk;
					break;
				}
			}
		}
	}
}

void Editor::Save() {
	/*std::ofstream myFile("level.txt");
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int column = 0; column < MAP_WIDTH; column++) {
			myFile << map[row][column];
			if (column == MAP_WIDTH - 1) continue;
			myFile << ",";
		}
		myFile << std::endl;
	}
	myFile.close();*/
}

void Editor::Load() {
	/*char tile;
	std::fstream in_level;
	in_level.open("level.txt");
	if (!in_level.is_open()) return;
	for (int row = 0; row < MAP_HEIGHT; row++) {
		for (int column = 0; column < MAP_WIDTH; column++) {
			tile = in_level.get();
			m_data[row][column] = atoi(&tile);
			in_level.ignore();
		}
	}*/
}