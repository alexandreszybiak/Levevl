#include "Input.h"

void Input::BeginNewFrame() {
	m_pressedKeys.clear();
	m_releasedKeys.clear();
	m_pressedMouseButtons.clear();
	m_releasedMouseButtons.clear();
	m_mouseWheel = 0;
}

// Store keyboard event key code
void Input::KeyDownEvent(const SDL_Event& event) {
	m_pressedKeys[event.key.keysym.scancode] = true;
	m_heldKeys[event.key.keysym.scancode] = true;
}

void Input::KeyUpEvent(const SDL_Event& event) {
	m_releasedKeys[event.key.keysym.scancode] = true;
	m_heldKeys[event.key.keysym.scancode] = false;
}

// Store mouse button events
void Input::MouseButtonDownEvent(const SDL_MouseButtonEvent& event) {
	m_pressedMouseButtons[event.button] = true;
	m_heldMouseButtons[event.button] = true;
	m_mouseX = event.x;
	m_mouseY = event.y;
}

void Input::MouseButtonUpEvent(const SDL_MouseButtonEvent& event) {
	m_releasedMouseButtons[event.button] = true;
	m_heldMouseButtons[event.button] = false;
	m_mouseX = event.x;
	m_mouseY = event.y;
}

void Input::MouseMotionEvent(const SDL_MouseMotionEvent& event) {
	m_mouseX = event.x;
	m_mouseY = event.y;
}

void Input::MouseWheelEvent(const SDL_MouseWheelEvent& event) {
	m_mouseWheel = event.y;
}

// Function to actually call in update functions
bool Input::WasKeyPressed(SDL_Scancode key) {
	return m_pressedKeys[key];
}

bool Input::WasKeyReleased(SDL_Scancode key) {
	return m_releasedKeys[key];
}

bool Input::IsKeyHeld(SDL_Scancode key) {
	return m_heldKeys[key];
}

bool Input::WasMouseButtonPressed(Uint8 mouseButton) {
	return m_pressedMouseButtons[mouseButton];
}

bool Input::WasMouseButtonReleased(Uint8 mouseButton) {
	return m_releasedMouseButtons[mouseButton];
}

bool Input::IsMouseButtonHeld(Uint8 mouseButton) {
	return m_heldMouseButtons[mouseButton];
}

// Get mouse position
int Input::GetMouseX() {
	return m_mouseX;
}

int Input::GetMouseY() {
	return m_mouseY;
}

// Get mouse wheel
int Input::GetMouseWheel() {
	if (m_mouseWheel == 0) return 0;
	if (m_mouseWheel > 0) return 1;
	if (m_mouseWheel < 0) return -1;
}