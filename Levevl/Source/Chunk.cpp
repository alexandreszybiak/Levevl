#pragma once
#include <vector>
#include <iostream>
#include "SDL.h"
#include "Graphics.h"
#include "Chunk.h"
#include "Game.h"
#include "Level.h"


Chunk::Chunk(int x, int y, int width, int height, char initValue, Level* levelRef):
		m_x(x),
		m_y(y),
		m_width(width),
		m_height(height),
		m_levelRef(levelRef),
		m_emptyRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_brickRect({ TILE_SIZE,0,TILE_SIZE,TILE_SIZE }),
		m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE })
{
	m_data.reserve(m_width * m_height);
	for (int i = 0; i < m_width * m_height; i++) {
		m_data.push_back(initValue);
	}
	std::cout << "Chunk created." << std::endl;
}

Chunk::Chunk(const Chunk& chunkCopy): 
		m_x(chunkCopy.m_x), 
		m_y(chunkCopy.m_y), 
		m_width(chunkCopy.m_width), 
		m_height(chunkCopy.m_height),
		m_levelRef(chunkCopy.m_levelRef),
		m_emptyRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_brickRect({ TILE_SIZE,0,TILE_SIZE,TILE_SIZE }),
		m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_data(chunkCopy.m_data) {
	std::cout << "Chunk copied." << std::endl;
}

Chunk::~Chunk() {
	m_data.clear();
	std::cout << "Chunk destroyed." << std::endl;
}

void Chunk::Fill(char value) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			if ((y * m_width + x) >= m_data.size()) {
				std::cout << "Value too high in Chunk::Fill" << std::endl;
				continue;
			}
			m_data[y * m_width + x] = value;
		}
	}
}

void Chunk::Draw(Graphics& graphics) {
	for (int column = 0; column < m_width; column++) {
		for (int row = 0; row < m_height; row++) {
			m_destinationRect.x = column * TILE_SIZE + m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y;
			if (m_data[column + row * m_width]  == 1) {
				graphics.Draw(graphics.chunkTexture, m_emptyRect, m_destinationRect);
			}
			else if (m_data[column + row * m_width] == 2) {
				graphics.Draw(graphics.chunkTexture, m_brickRect, m_destinationRect);
			}
		}
	}
}

void Chunk::DrawMask(Graphics& graphics) {
	for (int column = 0; column < m_width; column++) {
		for (int row = 0; row < m_height; row++) {
			SDL_Rect rect = { 0,0,TILE_SIZE,TILE_SIZE };
			// Look at neighbours horizontal
			if (column + 1 < m_width && m_data[column + 1 + row * m_width])
				rect.x += TILE_SIZE;
			if (column > 0 && m_data[column - 1 + row * m_width])
				rect.x += TILE_SIZE * 2;
			// Look at neighbours vertical
			if (row + 1 < m_height && m_data[column + (row + 1) * m_width])
				rect.y += TILE_SIZE;
			if (row > 0 && m_data[column + (row - 1) * m_width])
				rect.y += TILE_SIZE * 2;
			m_destinationRect.x = column * TILE_SIZE + m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y;
			if (m_data[column + row * m_width]) {
				graphics.Draw(graphics.chunkMaskTexture, rect, m_destinationRect);
			}
		}
	}
}
void Chunk::Move(int x, int y) {
	std::vector<Chunk*> otherChunks;
	for (Chunk& e : m_levelRef->v_chunks) {
		if (&e == this) continue;
		otherChunks.push_back(&e);
	}

	for (Chunk* e : otherChunks) {
		if (OverlapsChunk(e, x, y)) {
			e->m_x += x * TILE_SIZE;
			e->m_y += y * TILE_SIZE;
		}
	}
	m_x += x * TILE_SIZE;
	m_y += y * TILE_SIZE;
}

int Chunk::Edit(int mouseX, int mouseY, char value) {
	int tileX = floor(float(mouseX - m_x) / TILE_SIZE);
	int tileY = floor(float(mouseY - m_y) / TILE_SIZE);
	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
		return 0;
	m_data[tileX + tileY * m_width] = value;
	return 1;
}

int Chunk::GetX() {
	return m_x;
}

int Chunk::GetY() {
	return m_y;
}

int Chunk::GetWidth() {
	return m_width;
}

std::vector<char>* Chunk::GetData() {
	return &m_data;
}

bool Chunk::OverlapsPoint(int x, int y) {
	int tileX = floor(float(x - m_x) / TILE_SIZE);
	int tileY = floor(float(y - m_y) / TILE_SIZE);

	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height || m_data[tileX + tileY * m_width] == 0)
		return false;

	return true;
}

bool Chunk::OverlapsChunk(Chunk* otherChunk, int offsetX, int offsetY) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			if (m_data[x + y * m_width]) {
				if (otherChunk->OverlapsPoint((x + offsetX) * TILE_SIZE + m_x, (y + offsetY) * TILE_SIZE + m_y)) {
					return true;
				}
			}
		}
	}
	return false;
}

void Chunk::SetRegion(char value, int x1, int y1, int x2, int y2) {
	x1 -= m_x / TILE_SIZE;
	y1 -= m_y / TILE_SIZE;
	x2 -= m_x / TILE_SIZE;
	y2 -= m_y / TILE_SIZE;

	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			if (x < 0 || x >= m_width || y < 0 || y >= m_height)
				continue;
			m_data[x + y * m_width] = value;
		}
	}
}