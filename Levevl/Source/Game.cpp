#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Input.h"
#include "Chunk.h"
#include "Map.h"
#include "Game.h"
#include "Graphics.h"

Chunk* chunks[] = { nullptr };
Chunk* selectedChunk = nullptr;
Map* Game::worldMap = nullptr;

Game::Game() {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		std::cout << "Subsystems initialized!..." << std::endl;
	else
		return;

	Loop();
}

Game::~Game() {

	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}

void Game::Loop() {

	//Framerate
	const int FPS = 60;
	const int frameDuration = 1000 / 60;
	Uint32 frameStart;
	int frameTime;

	//Systems
	Input input;
	Graphics graphics;
	SDL_Event event;

	isRunning = true;

	//World
	worldMap = new Map();
	selectedChunk = chunks[0] = new Chunk(TILE_SIZE, TILE_SIZE);
	chunks[1] = new Chunk(9 * TILE_SIZE, TILE_SIZE);

	while (running()) {
		frameStart = SDL_GetTicks();

		// Listen for inputs
		input.BeginNewFrame();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.KeyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.KeyUpEvent(event);
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				input.MouseButtonDownEvent(event.button);
			}
			else if (event.type == SDL_MOUSEBUTTONUP) {
				input.MouseButtonUpEvent(event.button);
			}
			else if (event.type == SDL_QUIT) {
				isRunning = false;
				return;
			}
		}

		// Get elapsed time

		// Update
		update(input);
		graphics.Update(input);

		// Draw phase
		render(graphics);

		// Wait phase
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDuration) {
			SDL_Delay(frameDuration - frameTime);
		}
	}
}

void Game::update(Input& input) {



	// To be moved to Editor.cpp
	if (input.WasKeyPressed(SDL_SCANCODE_LEFT)) {
		selectedChunk->Move(-1, 0);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_RIGHT)) {
		selectedChunk->Move(1, 0);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_UP)) {
		selectedChunk->Move(0, -1);
	}
	if (input.WasKeyPressed(SDL_SCANCODE_DOWN)) {
		selectedChunk->Move(0, 1);
	}
	if (input.WasMouseButtonPressed(SDL_BUTTON_LEFT)) {
		if (!selectedChunk->Edit(input.GetMouseX(), input.GetMouseY(), 1)) {
			bool b_hasFoundChunk = false;
			for (int i = 0; i < MAX_CHUNK; i++) {
				if (!chunks[i]) continue;
				if (chunks[i]->Edit(input.GetMouseX(), input.GetMouseY(), 1)) {
					selectedChunk = chunks[i];
					b_hasFoundChunk = true;
					break;
				}
			}
			if (!b_hasFoundChunk) {
				worldMap->Edit(input.GetMouseX(), input.GetMouseY(), 1);
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
		if (!selectedChunk->Edit(input.GetMouseX(), input.GetMouseY(), 0)) {
			for (int i = 0; i < MAX_CHUNK; i++) {
				if (!chunks[i]) continue;
				if (chunks[i]->Edit(input.GetMouseX(), input.GetMouseY(), 0)) {
					selectedChunk = chunks[i];
					break;
				}
			}
		}
	}
}

void Game::render(Graphics& graphics) {
	SDL_BlendMode bm = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
	SDL_BlendMode bm2 = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);

	// Pre-draw
	SDL_SetRenderDrawBlendMode(graphics.m_renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(graphics.m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(graphics.m_renderer);

	// Begin render in texture
	SDL_SetRenderTarget(graphics.m_renderer, graphics.gameTexture);
	SDL_SetRenderDrawColor(graphics.m_renderer, 27, 28, 29, 255);
	SDL_RenderClear(graphics.m_renderer);

	//Draw world map
	worldMap->DrawMap(graphics);

	// Draw mask
	SDL_SetTextureBlendMode(graphics.chunkMaskTexture, bm);
	for (int i = 0; i < MAX_CHUNK; i++) {
		if (chunks[i])
			chunks[i]->DrawMask(graphics);
	}

	// Draw everything else
	SDL_SetTextureBlendMode(graphics.chunkTexture, bm2);
	for (int i = 0; i < MAX_CHUNK; i++) {
		if (chunks[i])
			chunks[i]->DrawMap(graphics);
	}


	// End draw
	SDL_SetRenderTarget(graphics.m_renderer, NULL);
	SDL_RenderCopy(graphics.m_renderer, graphics.gameTexture, &graphics.m_viewportRect, &graphics.m_viewportRect);
	SDL_RenderPresent(graphics.m_renderer);
}