#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Input.h"
#include "TextureManager.h"
#include "Chunk.h"
#include "Map.h"
#include "Game.h"


Input* Game::input = nullptr;
SDL_Renderer* Game::renderer = nullptr;
SDL_Texture* Game::chunkTexture = nullptr;
SDL_Texture* Game::chunkMaskTexture = nullptr;
SDL_Texture* Game::worldTexture = nullptr;
SDL_Texture* Game::gameTexture = nullptr;
int Game::mouseX = 0;
int Game::mouseY = 0;
bool Game::leftMouseButtonPressed = false;
bool Game::rightMouseButtonPressed = false;
bool Game::num1KeyPressed = false;
bool Game::num2KeyPressed = false;
bool Game::spaceKeyPressed = false;
bool Game::leftKeyPressed = false;
bool Game::rightKeyPressed = false;
bool Game::upKeyPressed = false;
bool Game::downKeyPressed = false;
bool Game::fullscreenKeyPressed = false;

Chunk* chunks[] = { nullptr };
Chunk* selectedChunk = nullptr;

Map* Game::worldMap = nullptr;

Game::Game() {
	int flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	flags = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialized!..." << std::endl;

		window = SDL_CreateWindow("Levevl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, flags);

		if (window) {
			std::cout << "Window created!" << std::endl;
			std::cout << SDL_GetWindowSurface(window)->w << "," << SDL_GetWindowSurface(window)->h << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 27, 28, 29, 255);
			std::cout << "Renderer created!" << std::endl;

			SDL_RendererInfo info;
			if (SDL_GetRendererInfo(renderer, &info) != 0)
			{
				std::cout << "Couldn't get RendererInfo: " << SDL_GetError() << std::endl;
			}
			else
			{
				std::cout << "SDL_Renderer: " << info.name << std::endl;
				std::cout << "SDL_RendererFlags: " << std::endl;
				if (info.flags & SDL_RENDERER_SOFTWARE) { std::cout << "  SDL_RENDERER_SOFTWARE" << std::endl; }
				if (info.flags & SDL_RENDERER_ACCELERATED) { std::cout << "  SDL_RENDERER_ACCELERATED" << std::endl; }
				if (info.flags & SDL_RENDERER_PRESENTVSYNC) { std::cout << "  SDL_RENDERER_PRESENTVSYNC" << std::endl; }
				if (info.flags & SDL_RENDERER_TARGETTEXTURE) { std::cout << "  SDL_RENDERER_TARGETTEXTURE" << std::endl; }
				std::cout << "Texture Formats: " << std::endl;
				for (size_t i = 0; i < info.num_texture_formats; ++i)
				{
					std::cout << "  " << SDL_GetPixelFormatName(info.texture_formats[i]) << std::endl;
				}
				std::cout << "Max Texture Width: " << info.max_texture_width << std::endl;
				std::cout << "Max Texture Height: " << info.max_texture_height << std::endl;
			}

			SDL_RenderSetLogicalSize(renderer, 640, 360);
			SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
		}
		viewportRect = { 0,0,640,360 };
		isRunning = true;
	}
	else {
		isRunning = false;
	}

	chunkTexture = TextureManager::LoadTexture("Assets/chunk_texture.png");
	chunkMaskTexture = TextureManager::LoadTexture("Assets/chunk_mask_tileset.tga");
	worldTexture = TextureManager::LoadTexture("Assets/world_texture.png");
	gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 640, 360);
	worldMap = new Map();
	selectedChunk = chunks[0] = new Chunk(TILE_SIZE, TILE_SIZE);
	chunks[1] = new Chunk(9 * TILE_SIZE, TILE_SIZE);

	Loop();
}

Game::~Game() {
	SDL_DestroyTexture(chunkTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}

void Game::Loop() {
	input = new Input();
	SDL_Event event;

	//Framerate
	const int FPS = 60;
	const int frameDuration = 1000 / 60;
	Uint32 frameStart;
	int frameTime;

	while (running()) {
		frameStart = SDL_GetTicks();

		Game::input->BeginNewFrame();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					Game::input->KeyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				Game::input->KeyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				isRunning = false;
				return;
			}
		}

		update();
		render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDuration) {
			SDL_Delay(frameDuration - frameTime);
		}
	}
}

void Game::update() {
	if (Game::input->WasKeyPressed(SDL_SCANCODE_LEFT)) {
		selectedChunk->Move(-1, 0);
	}
	else if (Game::input->WasKeyPressed(SDL_SCANCODE_RIGHT)) {
		selectedChunk->Move(1, 0);
	}
	else if (Game::input->WasKeyPressed(SDL_SCANCODE_UP)) {
		selectedChunk->Move(0, -1);
	}
	else if (Game::input->WasKeyPressed(SDL_SCANCODE_DOWN)) {
		selectedChunk->Move(0, 1);
	}
	if (leftMouseButtonPressed) {
		if (!selectedChunk->Edit(mouseX, mouseY, 1)) {
			bool b_hasFoundChunk = false;
			for (int i = 0; i < MAX_CHUNK; i++) {
				if (!chunks[i]) continue;
				if (chunks[i]->Edit(mouseX, mouseY, 1)) {
					selectedChunk = chunks[i];
					b_hasFoundChunk = true;
					break;
				}
			}
			if (!b_hasFoundChunk) {
				worldMap->Edit(mouseX, mouseY, 1);
				/*for (int i = 0; i < max_chunk; i++) {
					if (!chunks[i]) {
						selectedchunk = chunks[i] = new chunk(int(floor(mousex / tile_size) * tile_size), int(floor(mousey / tile_size) * tile_size));
						break;
					}
				}*/
			}
		}
	}
	else if (rightMouseButtonPressed) {
		if (!selectedChunk->Edit(mouseX, mouseY, 0)) {
			for (int i = 0; i < MAX_CHUNK; i++) {
				if (!chunks[i]) continue;
				if (chunks[i]->Edit(mouseX, mouseY, 0)) {
					selectedChunk = chunks[i];
					break;
				}
			}
		}
	}
	if (Game::input->WasKeyPressed(SDL_SCANCODE_F11)) {
		Uint32 flags = SDL_GetWindowFlags(window);
		Uint32 value = flags & SDL_WINDOW_FULLSCREEN_DESKTOP;
		if (value == 0)
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			SDL_SetWindowFullscreen(window, 0);
		int w = 0;
		int h = 0;
		SDL_GetRendererOutputSize(renderer, &w, &h);
		std::cout << "Renderer Output Size : " << w << "," << h << std::endl;
		SDL_Rect rect = { 0 };
		SDL_RenderGetViewport(renderer, &rect);
		std::cout << "Viewport : " << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << std::endl;
	}
}

void Game::render() {
	SDL_BlendMode bm = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
	SDL_BlendMode bm2 = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
	
	// Pre-draw
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	// Begin render in texture
	SDL_SetRenderTarget(renderer, gameTexture);
	SDL_SetRenderDrawColor(renderer, 27, 28, 29, 255);
	SDL_RenderClear(renderer);

	//Draw world map
	worldMap->DrawMap();
	
	// Draw mask
	SDL_SetTextureBlendMode(chunkMaskTexture, bm);
	for (int i = 0; i < MAX_CHUNK; i++) {
		if (chunks[i])
			chunks[i]->DrawMask();
	}

	// Draw everything else
	SDL_SetTextureBlendMode(chunkTexture, bm2);
	for (int i = 0; i < MAX_CHUNK; i++) {
		if (chunks[i])
			chunks[i]->DrawMap();
	}
	
	// End draw
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderCopy(renderer, gameTexture, &viewportRect, &viewportRect);
	SDL_RenderPresent(renderer);
}

void Game::clean() {
	SDL_DestroyTexture(chunkTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}