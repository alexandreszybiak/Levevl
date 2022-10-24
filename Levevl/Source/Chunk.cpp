#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include "SDL.h"
#include "Utilities.h"
#include "Graphics.h"
#include "TileMap.h"
#include "Map.h"
#include "Camera.h"
#include "Chunk.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "TileHitFx.h"

Chunk::Chunk(int x, int y, int width, int height, TileMap* tileMap, Level* levelRef):
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
		m_tileMap(tileMap),
		m_levelRef(levelRef),
		m_emptyRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_brickRect({ TILE_SIZE,0,TILE_SIZE,TILE_SIZE }),
		m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE }),
		m_chunkMovementType(CHUNK_MOVE_TYPE_EASE),
		m_turboDirection({ 0,0 }){}

//Chunk::Chunk(const Chunk& src): 
//		m_x(src.m_x), 
//		m_y(src.m_y),
//		m_targetX(src.m_targetX),
//		m_targetY(src.m_targetY),
//		m_width(src.m_width), 
//		m_height(src.m_height),
//		m_levelRef(src.m_levelRef),
//		m_emptyRect({ 0,0,TILE_SIZE,TILE_SIZE }),
//		m_brickRect({ TILE_SIZE,0,TILE_SIZE,TILE_SIZE }),
//		m_destinationRect({ 0,0,TILE_SIZE,TILE_SIZE }),
//		m_tileMap(src.m_tileMap) {
//	std::cout << "Chunk copied." << std::endl;
//}

Chunk::~Chunk() {
	std::cout << "Chunk destroyed." << std::endl;
}

void Chunk::Update() {
	int distX = m_x - m_targetX;
	int distY = m_y - m_targetY;

	if (m_chunkMovementType == CHUNK_MOVE_TYPE_EASE) {
		m_velocityX = ceil(abs(distX) * .5f) * Sign(distX) * -1;
		m_velocityY = ceil(abs(distY) * .5f) * Sign(distY) * -1;
	}

	else if (m_chunkMovementType == CHUNK_MOVE_TYPE_LINEAR) {
		m_velocityX = -12 * Sign(distX);
		m_velocityY = -12 * Sign(distY);
	}

	Move(m_velocityX, m_velocityY);

	// Movement is over
	if (m_x == m_targetX && m_y == m_targetY && m_turboDirection != Vector2::Zero() ) {
		bool couldSlide = Slide(m_turboDirection);
		if (couldSlide == false) {
			m_turboDirection = Vector2::Zero();
			m_chunkMovementType = CHUNK_MOVE_TYPE_EASE;
		}
	}

}

void Chunk::Move(float x, float y) {
	m_xRemainder += x;
	m_yRemainder += y;

	int moveX = m_xRemainder;
	int moveY = m_yRemainder;

	if (moveX == 0 && moveY == 0)
		return;

	if (moveX != 0) {

		for (Entity* e : m_levelRef->m_entities) {
			if (e->IsRiding(*this)) {
				e->SetCarryAmount(moveX, 0);
			}
		}

		m_xRemainder -= moveX;
		m_x += moveX;

		for (Entity* e : m_levelRef->m_entities) {
			e->Carry();
			while (e->OverlapsSolidX(-Sign(moveX), 0)) {
				e->MoveInstant(Sign(moveX), 0);
			}
		}

		//// Carry entities
		//if (isPlayerRiding) {
		//	m_levelRef->player->MoveX(moveX);
		//}
		//// Push entities
		//else {

		//	while (m_levelRef->player->OverlapsSolidX(-Sign(moveX), 0)) {
		//		m_levelRef->player->MoveInstant(Sign(moveX), 0);
		//	}
		//}	
	}

	if (moveY != 0) {

		for (Entity* e : m_levelRef->m_entities) {
			if (e->IsRiding(*this)) {
				e->SetCarryAmount(0, moveY);
			}
		}

		m_yRemainder -= moveY;
		m_y += moveY;

		for (Entity* e : m_levelRef->m_entities) {
			e->Carry();
			while (e->OverlapsSolidY(-Sign(moveY), 0)) {
				e->MoveInstant(0, Sign(moveY));
			}
		}

	}
}

void Chunk::Draw(Graphics& graphics) {
	m_tileMap->Draw(graphics, m_x, m_y);
}

void Chunk::DrawMask(Graphics& graphics) {
	for (int column = 0; column < m_width; column++) {
		for (int row = 0; row < m_height; row++) {
			SDL_Rect rect = { 0,0,TILE_SIZE,TILE_SIZE };
			// Look at neighbours horizontal
			if (column + 1 < m_width && m_tileMap->GetTile(column + 1, row)->Visible())
				rect.x += TILE_SIZE;
			if (column > 0 && m_tileMap->GetTile(column - 1, row)->Visible())
				rect.x += TILE_SIZE * 2;
			// Look at neighbours vertical
			if (row + 1 < m_height && m_tileMap->GetTile(column, row + 1)->Visible())
				rect.y += TILE_SIZE;
			if (row > 0 && m_tileMap->GetTile(column, row - 1)->Visible())
				rect.y += TILE_SIZE * 2;
			m_destinationRect.x = column * TILE_SIZE + m_x;
			m_destinationRect.y = row * TILE_SIZE + m_y;
			if (m_tileMap->GetTile(column, row)->Visible()) {
				graphics.Draw(graphics.chunkMaskTexture, &rect, &m_destinationRect);
			}
		}
	}
}
bool Chunk::Slide(const Vector2& direction, bool turbo) {
	std::vector<Chunk*> otherChunks;
	std::vector<Chunk*> pushedChunks;
	otherChunks.reserve(m_levelRef->v_chunks.size());
	pushedChunks.reserve(m_levelRef->v_chunks.size());
	for (Chunk& e : m_levelRef->v_chunks) {
		if (&e == this)
			continue;
		otherChunks.push_back(&e);
	}

	if (CanSlide(direction, otherChunks, pushedChunks)) {
		for (Chunk* e : pushedChunks) {
			e->m_targetX += direction.x * TILE_SIZE;
			e->m_targetY += direction.y * TILE_SIZE;
		}

		if (turbo) {
			m_turboDirection = direction;
			m_chunkMovementType = CHUNK_MOVE_TYPE_LINEAR;
		}

		return true;
	}

	return false;
}

bool Chunk::CanSlide(const Vector2& direction, std::vector<Chunk*>& otherChunks, std::vector<Chunk*>& pushedChunks) {
	if (OverlapsWalls(direction.x, direction.y))
		return false;

	for (int i = otherChunks.size() - 1; i >= 0; i--) {
		if (i >= otherChunks.size())
			continue;
		Chunk* currentChunk = otherChunks[i];
		if (OverlapsChunk(currentChunk, direction.x, direction.y)) {
			auto end = std::remove(otherChunks.begin(), otherChunks.end(), currentChunk);
			otherChunks.erase(end, otherChunks.end());
			if (!currentChunk->CanSlide(direction, otherChunks, pushedChunks))
				return false;
		}
	}

	pushedChunks.push_back(this);
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
	return nullptr;
}

int Chunk::ValueAtPoint(int x, int y) {
	int tileX = floor(float(x - m_x) / TILE_SIZE);
	int tileY = floor(float(y - m_y) / TILE_SIZE);

	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
		return 0;

	return m_tileMap->GetTile(tileX, tileY)->GetType();
}

int Chunk::OverlapsPoint(int x, int y) {
	int tileX = floor(float(x - m_x) / TILE_SIZE);
	int tileY = floor(float(y - m_y) / TILE_SIZE);

	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
		return 0;

	return m_tileMap->GetTile(tileX, tileY)->GetType();
}

bool Chunk::OverlapsChunk(Chunk* otherChunk, int offsetX = 0, int offsetY = 0) {
	for (int x = 0; x < m_width; x++) {
		for (int y = 0; y < m_height; y++) {
			if (m_tileMap->GetTile(x, y)->Visible()) {
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
			if (m_tileMap->GetTile(x, y)->Visible()) {
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

	m_tileMap->SetRegion(m_levelRef->m_tileTypes[value], x1, y1, x2, y2);

}

TileType* Chunk::TileAtPoint(int x, int y) {
	int tileX = floor(float(x - m_x) / TILE_SIZE);
	int tileY = floor(float(y - m_y) / TILE_SIZE);

	if (tileX < 0 || tileX >= m_width || tileY < 0 || tileY >= m_height)
		return 0;

	return m_tileMap->GetTile(tileX, tileY);
}