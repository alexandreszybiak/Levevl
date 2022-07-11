#include <iostream>
#include <algorithm>
#include "SDL.h"
#include "Game.h"
#include "Input.h"
#include "Graphics.h"
#include "Editor.h"
#include "Map"
#include "Chunk.h"
#include "Level.h"

Editor::Editor(Level* level) : m_brushValue(1), m_selection({0,0,0,0}) {
	m_level_ref = level;
	m_selectedChunk = nullptr;
	m_selectedChunkIndex = 0;
}

Editor::~Editor() {

}

void Editor::Update(Input& input) {
	if (input.WasKeyPressed(SDL_SCANCODE_LEFT)) {
		if(m_selectedChunk)
			m_selectedChunk->Move(-1, 0);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_RIGHT)) {
		if (m_selectedChunk)
			m_selectedChunk->Move(1, 0);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_UP)) {
		if (m_selectedChunk)
			m_selectedChunk->Move(0, -1);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_DOWN)) {
		if (m_selectedChunk)
			m_selectedChunk->Move(0, 1);
	}

	if (input.WasMouseButtonPressed(SDL_BUTTON_LEFT)) {
		// Initialize the selection
		m_selection.x = input.GetMouseX();
		m_selection.y = input.GetMouseY();

		// Reset the selected chunk
		m_selectedChunk = nullptr;

		// Look for the first chunk
		int index = m_level_ref->v_chunks.size() - 1;
		for (std::vector<Chunk>::reverse_iterator i = m_level_ref->v_chunks.rbegin();
			i != m_level_ref->v_chunks.rend(); ++i) {
			if (i->OverlapsPoint(input.GetMouseX(), input.GetMouseY())) {
				m_selectedChunk = &*i;
				m_selectedChunkIndex = index;
				break;
			}
			index--;
		}	
	}
	if (input.WasMouseButtonReleased(SDL_BUTTON_LEFT)) {
		//
		int gridX = std::min(input.GetMouseX(), m_selection.x) / TILE_SIZE;
		int gridY = std::min(input.GetMouseY(), m_selection.y) / TILE_SIZE;

		int gridX2 = ceil(float(std::max(input.GetMouseX(), m_selection.x)) / TILE_SIZE);
		int gridY2 = ceil(float(std::max(input.GetMouseY(), m_selection.y)) / TILE_SIZE);
		if (m_selectedChunk) {
			// Draw over the selected chunk
			m_selectedChunk->SetRegion(gridX, gridY, gridX2, gridY2);
		}
		else {
			m_selectedChunk = m_level_ref->BuildChunk(gridX * TILE_SIZE, gridY * TILE_SIZE, gridX2 - gridX, gridY2 - gridY);
			m_selectedChunkIndex = m_level_ref->v_chunks.size() - 1;
		}

		// Reset the selection
		m_selection.x = m_selection.y = m_selection.w = m_selection.h = 0;
	}
	if (input.IsMouseButtonHeld(SDL_BUTTON_LEFT)) {
		m_selection.w = input.GetMouseX() - m_selection.x;
		m_selection.h = input.GetMouseY() - m_selection.y;
	}
	
	if (input.WasKeyPressed(SDL_SCANCODE_DELETE)) {
		if (m_selectedChunk) {
			m_level_ref->DeleteChunk(m_selectedChunkIndex);
			m_selectedChunk = nullptr;
			m_selectedChunkIndex = 0;
		}
	}
}

void Editor::Draw(Graphics& graphics) {
	SDL_SetRenderDrawColor(graphics.m_renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(graphics.m_renderer, &m_selection);
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