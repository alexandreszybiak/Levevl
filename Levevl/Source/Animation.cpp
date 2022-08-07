#include "SDL.h"
#include "Animation.h"

int Animation::Update() {
	if (!Playing())
		return -1;

	if (SDL_GetTicks() - m_lastFrameTime > m_frameDuration) {
		m_lastFrameTime = SDL_GetTicks();

		if (!m_loop && m_iterator + 1 == GetLength()) {
			m_playing = false;
			return -1;
		}
		else{
			m_iterator = (m_iterator + 1) % GetLength();
			return GetFrame();
		}

	}

	return -1;
}

void Animation::PushFrame(int frame) {
	m_frames.push_back(frame);
}

void Animation::Reset() {
	m_playing = true;
	m_iterator = 0;
	m_lastFrameTime = SDL_GetTicks();
}