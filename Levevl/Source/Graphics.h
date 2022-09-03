#pragma once

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Rect;
class Input;
class Camera;

class Graphics {
public:
	Graphics();
	~Graphics();
	void Update(Input& input);
	SDL_Texture* LoadTexture(const char* fileName);
	void Draw(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destinationRect, SDL_RendererFlip flip = SDL_FLIP_NONE, double angle = 0, const SDL_Point* center = NULL);
private:
	void ToggleFullscreen();
public:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

	Camera& m_camera;
	SDL_Rect m_viewportRect;

	SDL_Texture* chunkTexture;
	SDL_Texture* chunkMaskTexture;
	SDL_Texture* worldTexture;
	SDL_Texture* gameTexture;
	SDL_Texture* playerTexture;
	SDL_Texture* playerBodyTexture;
	SDL_Texture* playerStickTexture;
	SDL_Texture* backgroundTexture;
	SDL_Texture* tileHitFxTexture;

private:
	//
};