#pragma once
#include "SDL.h"
#include "TextureManager.h"
#include "Chunk.h"
#include "Game.h"


Chunk::Chunk(int x, int y) : m_x(0), m_y(0), m_data() {
	Fill(2);
	m_x = x;
	m_y = y;
	m_texture = Game::chunkTexture;
	m_emptyRect = { 0,0,TILE_SIZE,TILE_SIZE };
	m_brickRect = { TILE_SIZE,0,TILE_SIZE,TILE_SIZE };
	m_destinationRect = { 0,0,TILE_SIZE,TILE_SIZE };
}
Chunk::~Chunk() {

}
void Chunk::Fill(char value) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			m_data[x][y] = value;
		}
	}
}
void Chunk::DrawMap() {
	for (int column = 0; column < m_width; column++) {
		for (int row = 0; row < m_height; row++) {
			m_destinationRect.x = column * TILE_SIZE + m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y;
			if (m_data[column][row] == 1) {
				TextureManager::Draw(m_texture, m_emptyRect, m_destinationRect);
			}
			else if (m_data[column][row] == 2) {
				TextureManager::Draw(m_texture, m_brickRect, m_destinationRect);
			}
		}
	}
}
void Chunk::Move(int x, int y) {
	m_x += x * TILE_SIZE;
	m_y += y * TILE_SIZE;
}

int Chunk::Edit(int mouseX, int mouseY) {
	char tile_x = char(floor((mouseX - m_x) / TILE_SIZE));
	char tile_y = char(floor((mouseY - m_y) / TILE_SIZE));
	if (tile_x < 0 || tile_x >= m_width || tile_y < 0 || tile_y >= m_height)
		return 0;
	m_data[tile_x][tile_y] = 1;
	return 1;
}