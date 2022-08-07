#pragma once
#include <vector>
#include "SDL.h"

class Animation {
private :
	int m_iterator;
	int m_lastFrameTime;
	bool m_loop;
	bool m_playing;

	// The frame duration in milliseconds
	const int m_frameDuration = 83;

	std::vector<Uint8> m_frames;
public:
	Animation(int length, bool loop = true) : m_iterator(0), m_lastFrameTime(0), m_loop(loop), m_playing(false) { m_frames.reserve(length); }
	
	int Update();

	void PushFrame(int index);
	void Reset();

	const int GetLength() { return m_frames.size(); }
	const int GetFrame() { return m_frames[m_iterator]; }
	const int GetIterator() { return m_iterator; }
	const int Playing() { return m_playing; }

	void Play() { m_playing = true; }

	void SetIterator(const int i) { m_iterator = i; }

};