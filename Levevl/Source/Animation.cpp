#include "SDL.h"
#include "Animation.h"

const KeyFrame* Animation::Update() {
	if (!Playing())
		return nullptr;

	if (SDL_GetTicks() - m_lastFrameTime > m_frameDuration) {
		m_lastFrameTime = SDL_GetTicks();

		if (!m_loop && m_iterator + 1 == GetLength()) {
			m_playing = false;
			return nullptr;
		}
		else{
			m_iterator = (m_iterator + 1) % GetLength();
			return GetFrame();
		}

	}

	return nullptr;
}

void Animation::PushFrame(int frameIndex, int duration) {
	m_frames.emplace_back(frameIndex, duration);
}

void Animation::Reset() {
	m_playing = true;
	m_iterator = 0;
	m_lastFrameTime = SDL_GetTicks();
}