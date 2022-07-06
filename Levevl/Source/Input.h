#pragma once
#include "SDL.h"

#include <map>

class Input {
public:
	void BeginNewFrame();
	void KeyUpEvent(const SDL_Event& event);
	void KeyDownEvent(const SDL_Event& event);

	bool WasKeyPressed(SDL_Scancode key);
	bool WasKeyReleased(SDL_Scancode key);
	bool IsKeyHeld(SDL_Scancode key);
private:
	std::map<SDL_Scancode, bool> m_heldKeys;
	std::map<SDL_Scancode, bool> m_pressedKeys;
	std::map<SDL_Scancode, bool> m_releasedKeys;
};