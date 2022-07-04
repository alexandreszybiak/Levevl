#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "TextureManager.h"
#include "Chunk.h"
#include "Game.h"



SDL_Renderer* Game::renderer = nullptr;
SDL_Texture* Game::chunkTexture = nullptr;
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

Game::Game() {
}

Game::~Game() {

}

void Game::init(const char* title, int xpos, int ypos, int width, int height) {
	int flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	flags = 0;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Subsystems initialized!..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

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
	gameTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 640, 360);
	selectedChunk = chunks[0] = new Chunk(0, 0);
	chunks[1] = new Chunk(6 * TILE_SIZE, 0);
}

void Game::handleEvents() {
	leftMouseButtonPressed = false;
	rightMouseButtonPressed = false;
	num1KeyPressed = false;
	num2KeyPressed = false;
	spaceKeyPressed = false;
	leftKeyPressed = false;
	rightKeyPressed = false;
	upKeyPressed = false;
	downKeyPressed = false;
	fullscreenKeyPressed = false;
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		mouseX = event.button.x;
		mouseY = event.button.y;
		if (event.button.button == SDL_BUTTON_LEFT) {
			leftMouseButtonPressed = true;
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			rightMouseButtonPressed = true;
		}
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_1) {
			num1KeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_2) {
			num2KeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_SPACE) {
			spaceKeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_LEFT) {
			leftKeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT) {
			rightKeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_UP) {
			upKeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_DOWN) {
			downKeyPressed = true;
		}
		else if (event.key.keysym.sym == SDLK_F11) {
			fullscreenKeyPressed = true;
		}
		break;
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update() {
	if (leftKeyPressed) {
		selectedChunk->Move(-1, 0);
	}
	else if (rightKeyPressed) {
		selectedChunk->Move(1, 0);
	}
	else if (upKeyPressed) {
		selectedChunk->Move(0, -1);
	}
	else if (downKeyPressed) {
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
				for (int i = 0; i < MAX_CHUNK; i++) {
					if (!chunks[i]) {
						selectedChunk = chunks[i] = new Chunk(int(floor(mouseX / TILE_SIZE) * TILE_SIZE), int(floor(mouseY / TILE_SIZE) * TILE_SIZE));
						break;
					}
				}
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
	if (fullscreenKeyPressed) {
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
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
	
	
	SDL_SetRenderTarget(renderer, gameTexture);
	SDL_SetRenderDrawColor(renderer, 27, 28, 29, 255);
	SDL_RenderFillRect(renderer, &viewportRect);
	SDL_RenderClear(renderer);

	SDL_Rect rect = { 24,24,24,24 };
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
	SDL_BlendMode bm = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);
	SDL_SetRenderDrawBlendMode(renderer, bm);
	SDL_RenderFillRect(renderer, &rect);
	SDL_BlendMode bm2 = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_DST_ALPHA, SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD);
	SDL_SetTextureBlendMode(chunkTexture, bm2);
	for (int i = 0; i < MAX_CHUNK; i++) {
		if (chunks[i])
			chunks[i]->DrawMap();
	}
	SDL_SetTextureBlendMode(chunkTexture, SDL_BLENDMODE_NONE);
	SDL_SetTextureBlendMode(gameTexture, SDL_BLENDMODE_NONE);
	//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	

	SDL_SetRenderTarget(renderer, NULL);

	SDL_RenderCopy(renderer, gameTexture, &viewportRect, &viewportRect);

	SDL_RenderPresent(renderer);
	//isRunning = false;
}

void Game::clean() {
	SDL_DestroyTexture(chunkTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned!" << std::endl;
}