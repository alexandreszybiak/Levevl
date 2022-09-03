#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "SDL.h"
#include "Utilities.h"
#include "Graphics.h"
#include "Camera.h"
#include "Chunk.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "TileHitFx.h"

Chunk::Chunk(int x, int y, int width, int height, char initValue, Level* levelRef):
		m_x(x),
		m_y(y),
		m_xRemainder(.0f),
		m_yRemainder(.0f),
		m_velocityX(.0f),
		m_velocityY(.0f),
		m_targetX(x),
		m_targetY(y),
		m_width(width),
		m_height(height),
		m_levelRef(levelRef),
		m_emptyRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_brickRect({ TILE_SIZE,0,TILE_SIZE,TILE_SIZE }),
		m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_tileHitFx(nullptr)
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
		m_targetX(chunkCopy.m_targetX),
		m_targetY(chunkCopy.m_targetY),
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

void Chunk::Update() {
	int distX = m_x - m_targetX;
	int distY = m_y - m_targetY;

	m_velocityX = ceil(abs(distX) * .5f) * Sign(distX) * -1;
	m_velocityY = ceil(abs(distY) * .5f) * Sign(distY) * -1;

	Move(m_velocityX, m_velocityY);
}

void Chunk::Move(float x, float y) {
	m_xRemainder += x;
	m_yRemainder += y;

	int moveX = m_xRemainder;
	int moveY = m_yRemainder;

	if (moveX == 0 && moveY == 0)
		return;

	if (moveX != 0) {
		bool isPlayerRiding = false;
		if (m_levelRef->player->IsRiding(*this)) {
			isPlayerRiding = true;
		}
		m_xRemainder -= moveX;
		m_x += moveX;
		if (m_tileHitFx)
			m_tileHitFx->Move(moveX, 0);

		if (isPlayerRiding) {
			m_levelRef->player->MoveX(moveX);
		}
		
	}

	if (moveY != 0) {
		bool isPlayerRiding = false;
		if (m_levelRef->player->IsRiding(*this)) {
			isPlayerRiding = true;
		}

		m_yRemainder -= moveY;
		m_y += moveY;
		if(m_tileHitFx)
			m_tileHitFx->Move(0, moveY);

		if (isPlayerRiding) {
			m_levelRef->player->MoveY(moveY);
		}
	}
}

void Chunk::Draw(Graphics& graphics) {
	for (int column = 0; column < m_width; column++) {
		for (int row = 0; row < m_height; row++) {
			m_destinationRect.x = column * TILE_SIZE + m_x - graphics.m_camera.m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y - graphics.m_camera.m_y;
			if (m_data[column + row * m_width]  == 1) {
				graphics.Draw(graphics.chunkTexture, &m_emptyRect, &m_destinationRect);
			}
			else if (m_data[column + row * m_width] == 2) {
				graphics.Draw(graphics.chunkTexture, &m_brickRect, &m_destinationRect);
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
			m_destinationRect.x = column * TILE_SIZE + m_x - graphics.m_camera.m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y - graphics.m_camera.m_y;
			if (m_data[column + row * m_width]) {
				graphics.Draw(graphics.chunkMaskTexture, &rect, &m_destinationRect);
			}
		}
	}
}
void Chunk::Slide(int x, int y) {
	std::vector<Chunk*> otherChunks;
	std::vector<Chunk*> freeChunks;
	otherChunks.reserve(m_levelRef->v_chunks.size());
	freeChunks.reserve(m_levelRef->v_chunks.size());
	for (Chunk& e : m_levelRef->v_chunks) {
		if (&e == this)
			continue;
		otherChunks.push_back(&e);
	}

	if (CanSlide(x, y, otherChunks, freeChunks)) {
		for (Chunk* e : freeChunks) {
			e->m_targetX += x * TILE_SIZE;
			e->m_targetY += y * TILE_SIZE;
		}
	}
}

bool Chunk::CanSlide(int x, int y, std::vector<Chunk*>& otherChunks, std::vector<Chunk*>& freeChunks) {
	if (OverlapsWalls(x, y))
		return false;

	for (int i = otherChunks.size() - 1; i >= 0; i--) {
		if (i >= otherChunks.size())
			continue;
		Chunk* currentChunk = otherChunks[i];
		if (OverlapsChunk(currentChunk, x, y)) {
			auto end = std::remove(otherChunks.begin(), otherChunks.end(), currentChunk);
			otherChunks.erase(end, otherChunks.end());
			if (!currentChunk->CanSlide(x, y, otherChunks, freeChunks))
				return false;
		}
	}

	freeChunks.push_back(this);
	return true;
}

int Chunk::GetX() {
	return m_targetX;
}

int Chunk::GetY() {
	return m_targetY;
}

int Chunk::GetWidth() {
	return m_width;
}

std::vector<char>* Chunk::GetData() {
	return &m_data;
}

int Chunk::ValueAtPoint(int x, int y) {
	int tileX = floor(float(x - m_x) / TILE_SIZE);
	int tileY = floor(float(y - m_y) / TILE_SIZE);

	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
		return 0;

	return m_data[tileX + tileY * m_width];
}

int Chunk::OverlapsPoint(int x, int y) {
	int tileX = floor(float(x - m_x) / TILE_SIZE);
	int tileY = floor(float(y - m_y) / TILE_SIZE);

	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
		return 0;

	return m_data[tileX + tileY * m_width];
}

bool Chunk::OverlapsChunk(Chunk* otherChunk, int offsetX = 0, int offsetY = 0) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			if (m_data[x + y * m_width]) {
				if (otherChunk->OverlapsPoint((x + offsetX) * TILE_SIZE + m_targetX, (y + offsetY) * TILE_SIZE + m_targetY)) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Chunk::OverlapsWalls(int offsetX = 0, int offsetY = 0) {
	int px = 0;
	int py = 0;
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			if (m_data[x + y * m_width]) {
				px = (x + offsetX) * TILE_SIZE + m_targetX;
				py = (y + offsetY) * TILE_SIZE + m_targetY;
				if(m_levelRef->worldMap->OverlapsPoint(px, py))
					return true;
			}
		}
	}
	return false;
}

void Chunk::SetRegion(char value, int x1, int y1, int x2, int y2) {
	x1 -= m_targetX / TILE_SIZE;
	y1 -= m_targetY / TILE_SIZE;
	x2 -= m_targetX / TILE_SIZE;
	y2 -= m_targetY / TILE_SIZE;

	for (int x = x1; x < x2; x++) {
		for (int y = y1; y < y2; y++) {
			if (x < 0 || x >= m_width || y < 0 || y >= m_height)
				continue;
			m_data[x + y * m_width] = value;
		}
	}
}