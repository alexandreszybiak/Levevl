#pragma once

class TextureManager {
public:
	static SDL_Texture* LoadTexture(const char* fileName);
	static void Draw(SDL_Texture* texture, SDL_Rect sourceRect, SDL_Rect destinationRect);
};