#pragma once
#include "SDL.h"

#include <map>

class Level;

class Input {
public:
	SDL_GameController* m_gameController;

private:
	std::map<Uint8, bool> m_heldControllerButtons;
	std::map<Uint8, bool> m_pressedControllerButtons;
	std::map<Uint8, bool> m_releasedControllerButtons;
	std::map<SDL_Scancode, bool> m_heldKeys;
	std::map<SDL_Scancode, bool> m_pressedKeys;
	std::map<SDL_Scancode, bool> m_releasedKeys;
	std::map<Uint8, bool> m_heldMouseButtons;
	std::map<Uint8, bool> m_pressedMouseButtons;
	std::map<Uint8, bool> m_releasedMouseButtons;
	int m_mouseX;
	int m_mouseY;
	int m_mouseWheel;

public:
	Input();
	~Input();
	void BeginNewFrame();

	void ControllerButtonUpEvent(const SDL_ControllerButtonEvent& event);
	void ControllerButtonDownEvent(const SDL_ControllerButtonEvent& event);

	void KeyUpEvent(const SDL_Event& event);
	void KeyDownEvent(const SDL_Event& event);

	void MouseButtonUpEvent(const SDL_MouseButtonEvent& event);
	void MouseButtonDownEvent(const SDL_MouseButtonEvent& event);
	void MouseMotionEvent(const SDL_MouseMotionEvent& event);
	void MouseWheelEvent(const SDL_MouseWheelEvent& event);

	bool WasControllerButtonPressed(Uint8 button);
	bool WasControllerButtonReleased(Uint8 button);
	bool IsControllerButtonHeld(Uint8 button);

	bool WasKeyPressed(SDL_Scancode key);
	bool WasKeyReleased(SDL_Scancode key);
	bool IsKeyHeld(SDL_Scancode key);

	bool WasMouseButtonPressed(Uint8 button);
	bool WasMouseButtonReleased(Uint8 button);
	bool IsMouseButtonHeld(Uint8 button);
	int GetMouseWindowX();
	int GetMouseWindowY();
	int GetMouseWorldX(Level& world);
	int GetMouseWorldY(Level& world);
	int GetMouseWheel();

private:
	
};