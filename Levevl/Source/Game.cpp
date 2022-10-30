#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Input.h"
#include "TileMap.h"
#include "Chunk.h"
#include "Map.h"
#include "Game.h"
#include "Graphics.h"
#include "Editor.h"
#include "Level.h"
#include "Player.h"
#include "TileHitFx.h"

Game::Game() {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
		std::cout << "Subsystems initialized!..." << std::endl;
	else
		return;

	//m_tileHitFx = *(new TileHitFx());

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

	level = new Level(graphics.m_camera);

	//std::cout << "v_chunks address is : " << &v_chunks << std::endl;

	// Editor
	Editor editor(level);

	while (running()) {
		frameStart = SDL_GetTicks();

		// Listen for inputs
		input.BeginNewFrame();

		while (SDL_PollEvent(&event)) {
			
			if (event.type == SDL_CONTROLLERBUTTONDOWN) {
				input.ControllerButtonDownEvent(event.cbutton);
			}
			else if (event.type == SDL_CONTROLLERBUTTONUP) {
				input.ControllerButtonUpEvent(event.cbutton);
			}
			else if (event.type == SDL_KEYDOWN) {
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
			else if (event.type == SDL_MOUSEMOTION) {
				input.MouseMotionEvent(event.motion);
			}
			else if (event.type == SDL_MOUSEWHEEL) {
				input.MouseWheelEvent(event.wheel);
			}
			else if (event.type == SDL_CONTROLLERDEVICEADDED) {
				input.ControllerDeviceAddedEvent(event.cdevice);
			}
			else if (event.type == SDL_CONTROLLERDEVICEREMOVED) {
				input.ControllerDeviceRemovedEvent(event.cdevice);
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
		editor.Update(input);

		// Draw phase
		render(graphics);
		editor.Draw(graphics);

		// End draw
		SDL_RenderPresent(graphics.m_renderer);

		// Wait phase
		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDuration) {
			SDL_Delay(frameDuration - frameTime);
		}
	}
}

void Game::update(Input& input) {

	level->Update(input);


}

void Game::render(Graphics& graphics) {
	SDL_BlendMode drawInAlpha = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
	SDL_BlendMode maskedByAlpha = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);

	SDL_BlendMode blendAndPreserveAlpha = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);

	// Pre-draw
	SDL_SetRenderDrawBlendMode(graphics.m_renderer, SDL_BLENDMODE_BLEND);

	// Begin render in texture
	SDL_SetRenderTarget(graphics.m_renderer, graphics.gameTexture);
	SDL_SetRenderDrawColor(graphics.m_renderer, 0, 0, 0, 0);
	SDL_RenderClear(graphics.m_renderer);

	// Draw mask
	SDL_SetTextureBlendMode(graphics.chunkMaskTexture, drawInAlpha);
	for (Chunk& chunk : level->v_chunks) {
		chunk.DrawMask(graphics);
	}

	// Draw everything else
	SDL_SetTextureBlendMode(graphics.chunkTexture, blendAndPreserveAlpha);
	SDL_SetTextureBlendMode(graphics.playerBodyTexture, blendAndPreserveAlpha);
	SDL_SetTextureBlendMode(graphics.playerStickTexture, blendAndPreserveAlpha);
	SDL_SetTextureBlendMode(graphics.tileHitFxTexture, blendAndPreserveAlpha);
	for (Chunk& chunk : level->v_chunks) {
		chunk.Draw(graphics);
	}
	level->DrawEntities(graphics);
	//level->player->Draw(graphics);
	//level->m_tileHitFx.Draw(graphics);

	// Draw the background
	SDL_SetTextureBlendMode(graphics.backgroundTexture, maskedByAlpha);
	level->worldMap->DrawBackground(graphics);

	//Draw world map
	level->worldMap->Draw(graphics);

	// End draw
	SDL_SetTextureBlendMode(graphics.gameTexture, SDL_BLENDMODE_NONE);
	SDL_SetRenderTarget(graphics.m_renderer, NULL);
	SDL_RenderCopy(graphics.m_renderer, graphics.gameTexture, &graphics.m_viewportRect, &graphics.m_viewportRect);

}