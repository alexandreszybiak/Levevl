#include "SDL.h"
#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {

	const int FPS = 60;
	const int frameDuration = 1000 / 60;

	Uint32 frameStart;
	int frameTime;

	Game game;

	game.init("Levevl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080);

	while (game.running()) {

		frameStart = SDL_GetTicks();

		game.handleEvents();
		game.update();
		game.render();

		frameTime = SDL_GetTicks() - frameStart;

		if (frameTime < frameDuration) {
			SDL_Delay(frameDuration - frameTime);
		}
	}


	return 0;
}