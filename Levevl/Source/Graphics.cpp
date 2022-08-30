#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Graphics.h"
#include "Input.h"

Graphics::Graphics() {
	int flags = SDL_WINDOW_FULLSCREEN_DESKTOP;
	flags = 0;
	m_window = SDL_CreateWindow("Levevl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, flags);

	if (m_window) {
		std::cout << "Window created!" << std::endl;
		std::cout << SDL_GetWindowSurface(m_window)->w << "," << SDL_GetWindowSurface(m_window)->h << std::endl;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	if (m_renderer) {
		std::cout << "Renderer created!" << std::endl;

		SDL_RendererInfo info;
		if (SDL_GetRendererInfo(m_renderer, &info) != 0)
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

		SDL_RenderSetLogicalSize(m_renderer, 640, 360);
		SDL_RenderSetIntegerScale(m_renderer, SDL_TRUE);
	}
	m_viewportRect = { 0,0,640,360 };

	//Load textures
	chunkTexture = LoadTexture("Assets/chunk_texture.png");
	chunkMaskTexture = LoadTexture("Assets/chunk_mask_tileset.tga");
	worldTexture = LoadTexture("Assets/world_texture.png");
	playerTexture = LoadTexture("Assets/player.png");
	playerBodyTexture = LoadTexture("Assets/player_body_texture.png");
	playerStickTexture = LoadTexture("Assets/player_stick_texture.png");
	backgroundTexture = LoadTexture("Assets/background_texture.png");
	tileHitFxTexture = LoadTexture("Assets/tile_hit_fx_texture.png");

	gameTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, 640, 360);
}

Graphics::~Graphics() {
	SDL_DestroyTexture(chunkTexture);
	SDL_DestroyTexture(chunkMaskTexture);
	SDL_DestroyTexture(worldTexture);
	SDL_DestroyTexture(gameTexture);
	SDL_DestroyTexture(playerTexture);
	SDL_DestroyTexture(backgroundTexture);
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	std::cout << "Graphics deconstructor" << std::endl;
}

void Graphics::Update(Input& input) {
	if (input.WasKeyPressed(SDL_SCANCODE_F11)) {
		ToggleFullscreen();
	}
}

SDL_Texture* Graphics::LoadTexture(const char* fileName) {
	SDL_Surface* tmpSurface = IMG_Load(fileName);
	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(m_renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return newTexture;
}
void Graphics::Draw(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destinationRect, SDL_RendererFlip flip, double angle, const SDL_Point* center)
{
	SDL_RenderCopyEx(m_renderer, texture, sourceRect, destinationRect, angle, center, flip);
}

void Graphics::ToggleFullscreen() {
	Uint32 flags = SDL_GetWindowFlags(m_window);
	Uint32 value = flags & SDL_WINDOW_FULLSCREEN_DESKTOP;
	if (value == 0)
		SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	else
		SDL_SetWindowFullscreen(m_window, 0);
	int w = 0;
	int h = 0;
	SDL_GetRendererOutputSize(m_renderer, &w, &h);
	std::cout << "Renderer Output Size : " << w << "," << h << std::endl;
	SDL_Rect rect = { 0 };
	SDL_RenderGetViewport(m_renderer, &rect);
	std::cout << "Viewport : " << rect.x << "," << rect.y << "," << rect.w << "," << rect.h << std::endl;
}