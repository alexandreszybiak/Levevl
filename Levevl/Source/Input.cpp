#include "Input.h"

void Input::BeginNewFrame() {
	m_pressedKeys.clear();
	m_releasedKeys.clear();
}

void Input::KeyDownEvent(const SDL_Event& event) {
	m_pressedKeys[event.key.keysym.scancode] = true;
	m_heldKeys[event.key.keysym.scancode] = true;
}

void Input::KeyUpEvent(const SDL_Event& event) {
	m_releasedKeys[event.key.keysym.scancode] = true;
	m_heldKeys[event.key.keysym.scancode] = false;
}

bool Input::WasKeyPressed(SDL_Scancode key) {
	return m_pressedKeys[key];
}

bool Input::WasKeyReleased(SDL_Scancode key) {
	return m_releasedKeys[key];
}

bool Input::IsKeyHeld(SDL_Scancode key) {
	return m_heldKeys[key];
}