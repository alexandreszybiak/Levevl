#pragma once
#include "SDL.h"

#include <map>

class Input {
public:
	void BeginNewFrame();
	void KeyUpEvent(const SDL_Event& event);
	void KeyDownEvent(const SDL_Event& event);
	void MouseButtonUpEvent(const SDL_MouseButtonEvent& event);
	void MouseButtonDownEvent(const SDL_MouseButtonEvent& event);
	void MouseMotionEvent(const SDL_MouseMotionEvent& event);
	bool WasKeyPressed(SDL_Scancode key);
	bool WasKeyReleased(SDL_Scancode key);
	bool IsKeyHeld(SDL_Scancode key);
	bool WasMouseButtonPressed(Uint8 button);
	bool WasMouseButtonReleased(Uint8 button);
	bool IsMouseButtonHeld(Uint8 button);
	int GetMouseX();
	int GetMouseY();

private:
	//

public:
	//

private:
	std::map<SDL_Scancode, bool> m_heldKeys;
	std::map<SDL_Scancode, bool> m_pressedKeys;
	std::map<SDL_Scancode, bool> m_releasedKeys;
	std::map<Uint8, bool> m_heldMouseButtons;
	std::map<Uint8, bool> m_pressedMouseButtons;
	std::map<Uint8, bool> m_releasedMouseButtons;
	int m_mouseX;
	int m_mouseY;
};