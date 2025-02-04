#include <iostream>
#include "SDL.h"
#include "Input.h"
#include "Level.h"
#include "Camera.h"

Input::Input() : m_gameController(NULL) {
	if (SDL_NumJoysticks() > 0) {
		m_gameController = SDL_GameControllerOpen(0);

		if (m_gameController == NULL) {
			std::cout << "Unable to open joystick" << std::endl;
		}
	}
	else {
		std::cout << "No joystick found" << std::endl;
	}

}

Input::~Input() {
	SDL_GameControllerClose(m_gameController);
	m_gameController = NULL;
}


void Input::BeginNewFrame() {
	m_pressedControllerButtons.clear();
	m_releasedControllerButtons.clear();
	m_pressedKeys.clear();
	m_releasedKeys.clear();
	m_pressedMouseButtons.clear();
	m_releasedMouseButtons.clear();
	m_mouseWheel = 0;
	m_mouseDeltaX = 0;
	m_mouseDeltaY = 0;
}
 // Store gamepad event
void Input::ControllerButtonDownEvent(const SDL_ControllerButtonEvent& event) {
	m_pressedControllerButtons[event.button] = true;
	m_heldControllerButtons[event.button] = true;
}

void Input::ControllerButtonUpEvent(const SDL_ControllerButtonEvent& event) {
	m_releasedControllerButtons[event.button] = true;
	m_heldControllerButtons[event.button] = false;
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
	m_mouseDeltaX = m_mouseX - event.x;
	m_mouseDeltaY = m_mouseY - event.y;
	m_mouseX = event.x;
	m_mouseY = event.y;
}

void Input::MouseWheelEvent(const SDL_MouseWheelEvent& event) {
	m_mouseWheel = event.y;
}

// Function to actually call in update functions
bool Input::WasControllerButtonPressed(Uint8 button) {
	return m_pressedControllerButtons[button];
}

bool Input::WasControllerButtonReleased(Uint8 button) {
	return m_releasedControllerButtons[button];
}

bool Input::IsControllerButtonHeld(Uint8 button) {
	return m_heldControllerButtons[button];
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
int Input::GetMouseWindowX() {
	return m_mouseX;
}

int Input::GetMouseWindowY() {
	return m_mouseY;
}

int Input::GetMouseWorldX(Level& world) {
	return m_mouseX + world.m_camera.m_x;
}

int Input::GetMouseWorldY(Level& world) {
	return m_mouseY + world.m_camera.m_y;
}

int Input::GetMouseDeltaX() {
	return m_mouseDeltaX;
}

int Input::GetMouseDeltaY() {
	return m_mouseDeltaY;
}

// Get mouse wheel
int Input::GetMouseWheel() {
	if (m_mouseWheel == 0) return 0;
	if (m_mouseWheel > 0) return 1;
	if (m_mouseWheel < 0) return -1;
}

void Input::ControllerDeviceAddedEvent(const SDL_ControllerDeviceEvent& event) {
	if (m_gameController == NULL) {
		m_gameController = SDL_GameControllerOpen(event.which);
		std::cout << "New controller device added" << std::endl;
	}
}

void Input::ControllerDeviceRemovedEvent(const SDL_ControllerDeviceEvent& event) {
	if (m_gameController == SDL_GameControllerFromInstanceID(event.which)) {
		SDL_GameControllerClose(m_gameController);
		m_gameController = NULL;
		std::cout << "Current controller device removed" << std::endl;
	}
}