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
	int m_mouseDeltaX;
	int m_mouseDeltaY;
	int m_mouseWheel;

public:
	Input();
	~Input();
	void BeginNewFrame();

	void ControllerDeviceAddedEvent(const SDL_ControllerDeviceEvent& event);
	void ControllerDeviceRemovedEvent(const SDL_ControllerDeviceEvent& event);

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
	int GetMouseDeltaX();
	int GetMouseDeltaY();
	int GetMouseWheel();

	bool HoldingLeft() {
		return IsKeyHeld(SDL_SCANCODE_LEFT) || IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	}
	bool HoldingRight() {
		return IsKeyHeld(SDL_SCANCODE_RIGHT) || IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
	}
	bool HoldingUp() {
		return IsKeyHeld(SDL_SCANCODE_UP) || IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_UP);
	}

	bool HoldingDown() {
		return IsKeyHeld(SDL_SCANCODE_DOWN) || IsControllerButtonHeld(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	}

	bool PressedUp() {
		return WasKeyPressed(SDL_SCANCODE_UP) || WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_UP);
	}
	bool PressedDown() {
		return WasKeyPressed(SDL_SCANCODE_DOWN) || WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	}
	bool PressedJump() {
		return WasKeyPressed(SDL_SCANCODE_SPACE) || WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_B);
	}
	bool PressedHit() {
		return WasKeyPressed(SDL_SCANCODE_X) || WasControllerButtonPressed(SDL_CONTROLLER_BUTTON_Y);
	}
	bool ReleasedJump() {
		return WasKeyReleased(SDL_SCANCODE_SPACE) || WasControllerButtonReleased(SDL_CONTROLLER_BUTTON_B);
	}
	
private:
	
};