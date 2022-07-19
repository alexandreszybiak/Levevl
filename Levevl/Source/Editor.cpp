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

Editor::Editor(Level* level) :
		m_brushValue(1),
		m_selection({0,0,0,0}),
		m_cursorX(0),
		m_cursorY(0),
		m_brushMode(BRUSH_MODE_DRAW),
		m_selectionOriginX(0),
		m_selectionOriginY(0) {
	m_level_ref = level;
	m_selectedChunk = nullptr;
	m_selectedChunkIndex = 0;
}

Editor::~Editor() {

}

void Editor::Update(Input& input) {
	m_cursorX = input.GetMouseX();
	m_cursorY = input.GetMouseY();

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

	if (input.WasMouseButtonPressed(SDL_BUTTON_LEFT) || input.WasMouseButtonPressed(SDL_BUTTON_RIGHT)) {
		// Initialize the selection
		m_selectionOriginX = input.GetMouseX();
		m_selectionOriginY = input.GetMouseY();

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

		if (input.WasMouseButtonPressed(SDL_BUTTON_LEFT))
			m_brushMode = BRUSH_MODE_DRAW;
		else
			m_brushMode = BRUSH_MODE_ERASE;

	}
	if (input.IsMouseButtonHeld(SDL_BUTTON_LEFT) || input.IsMouseButtonHeld(SDL_BUTTON_RIGHT)) {

		m_selection.x = std::min(input.GetMouseX(), m_selectionOriginX) / TILE_SIZE * TILE_SIZE;
		m_selection.y = std::min(input.GetMouseY(), m_selectionOriginY) / TILE_SIZE * TILE_SIZE;
		m_selection.w = abs(m_selection.x - std::max(input.GetMouseX(), m_selectionOriginX)) / TILE_SIZE * TILE_SIZE + TILE_SIZE;
		m_selection.h = abs(m_selection.y - std::max(input.GetMouseY(), m_selectionOriginY)) / TILE_SIZE * TILE_SIZE + TILE_SIZE;
	}
	else if (input.WasMouseButtonReleased(SDL_BUTTON_LEFT) || input.WasMouseButtonReleased(SDL_BUTTON_RIGHT)) {
		//
		int gridX = m_selection.x / TILE_SIZE;
		int gridY = m_selection.y / TILE_SIZE;

		int gridX2 = (m_selection.x + m_selection.w) / TILE_SIZE;
		int gridY2 = (m_selection.y + m_selection.h) / TILE_SIZE;
		if (m_selectedChunk) {
			// Draw over the selected chunk
			m_selectedChunk->SetRegion((m_brushValue + 1) * m_brushMode, gridX, gridY, gridX2, gridY2);
		}
		else if (m_brushValue == 2) {
			m_level_ref->worldMap->SetRegion(1 * m_brushMode, gridX, gridY, gridX2, gridY2);
		}
		else if(m_brushMode) {
			m_selectedChunk = m_level_ref->BuildChunk(gridX * TILE_SIZE, gridY * TILE_SIZE, gridX2 - gridX, gridY2 - gridY, m_brushValue + 1);
			m_selectedChunkIndex = m_level_ref->v_chunks.size() - 1;
		}

		// Reset the selection
		m_selection.x = m_selection.y = m_selection.w = m_selection.h = 0;

		//Save the level
		Save();
	}
	
	
	if (int y = input.GetMouseWheel()) {
		int maxValue = 3;
		m_brushValue = (m_brushValue + maxValue + y) % maxValue;
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
	
	// Draw the selection rectangle
	if (m_selection.w && m_selection.h) {
		SDL_SetRenderDrawColor(graphics.m_renderer, 255, 0, 0, 255);
		SDL_RenderDrawRect(graphics.m_renderer, &m_selection);
	}
	
	// Draw the current brush value
	SDL_Texture* cursorTexture = graphics.chunkTexture;
	SDL_Rect srcRect = { TILE_SIZE * m_brushValue,0,TILE_SIZE, TILE_SIZE };
	SDL_Rect dstRect = { m_cursorX + 8,m_cursorY + 12,TILE_SIZE, TILE_SIZE };

	if (m_brushValue > 1) {
		cursorTexture = graphics.worldTexture;
		srcRect = { 0, 0, TILE_SIZE, TILE_SIZE };
	}
	SDL_RenderCopy(graphics.m_renderer, cursorTexture, &srcRect, &dstRect);
}

void Editor::Save() {
	SDL_RWops* levelFile = SDL_RWFromFile("level.bin", "w+b");
	if (levelFile) {
		for (int i = 0; i < (m_level_ref->worldMap->m_mapWidth * m_level_ref->worldMap->m_mapHeight); i++) {
			SDL_RWwrite(levelFile, &m_level_ref->worldMap->m_data[i], sizeof(Uint8), 1);
		}
		
		SDL_RWclose(levelFile);
	}
}

void Editor::Load() {
	
}