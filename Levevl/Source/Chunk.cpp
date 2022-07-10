#pragma once
#include <vector>
#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "Chunk.h"
#include "Game.h"



Chunk::Chunk(int x, int y) : m_x(0), m_y(0) {
	m_data.push_back({ 1, 2, 2, 1 });
	m_data.push_back({ 1, 2, 1, 2 });
	Fill(2);
	m_x = x;
	m_y = y;
	m_emptyRect = { 0,0,TILE_SIZE,TILE_SIZE };
	m_brickRect = { TILE_SIZE,0,TILE_SIZE,TILE_SIZE };
	m_destinationRect = { 0,0,TILE_SIZE,TILE_SIZE };
	std::cout << "Chunk created." << std::endl;
}
Chunk::~Chunk() {

}

void Chunk::Fill(char value) {
	for (int x = 0; x < m_data.size(); x++) {
		for (int y = 0; y < m_data[x].size(); y++) {
			m_data[x][y] = value;
		}
	}
}

void Chunk::Draw(Graphics& graphics) {
	for (int column = 0; column < m_data.size(); column++) {
		for (int row = 0; row < m_data[column].size(); row++) {
			m_destinationRect.x = column * TILE_SIZE + m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y;
			if (m_data[column][row] == 1) {
				graphics.Draw(graphics.chunkTexture, m_emptyRect, m_destinationRect);
			}
			else if (m_data[column][row] == 2) {
				graphics.Draw(graphics.chunkTexture, m_brickRect, m_destinationRect);
			}
		}
	}
}

void Chunk::DrawMask(Graphics& graphics) {
	for (int column = 0; column < m_data.size(); column++) {
		for (int row = 0; row < m_data[column].size(); row++) {
			SDL_Rect rect = { 0,0,TILE_SIZE,TILE_SIZE };
			// Look at neighbours horizontal
			if (column + 1 < m_data.size() && m_data[column + 1][row])
				rect.x += TILE_SIZE;
			if (column > 0 && m_data[column - 1][row])
				rect.x += TILE_SIZE * 2;
			// Look at neighbours vertical
			if (row + 1 < m_data[column].size() && m_data[column][row + 1])
				rect.y += TILE_SIZE;
			if (row > 0 && m_data[column][row - 1])
				rect.y += TILE_SIZE * 2;
			m_destinationRect.x = column * TILE_SIZE + m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y;
			if (m_data[column][row]) {
				graphics.Draw(graphics.chunkMaskTexture, rect, m_destinationRect);
			}
		}
	}
}
void Chunk::Move(int x, int y) {
	m_x += x * TILE_SIZE;
	m_y += y * TILE_SIZE;
}

int Chunk::Edit(int mouseX, int mouseY, char value) {
	int tile_x = floor(float(mouseX - m_x) / TILE_SIZE);
	int tile_y = floor(float(mouseY - m_y) / TILE_SIZE);
	if (tile_x < 0 || tile_x >= m_data.size() || tile_y < 0 || tile_y >= m_data[0].size())
		return 0;
	m_data[tile_x][tile_y] = value;
	return 1;
}