#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};