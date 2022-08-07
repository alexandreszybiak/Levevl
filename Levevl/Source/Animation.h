#pragma once
#include <vector>
#include "SDL.h"
#include "KeyFrame.h"

class Animation {
private :
	int m_iterator;
	int m_lastFrameTime;
	bool m_loop;
	bool m_playing;

	// The frame duration in milliseconds
	const int m_frameDuration = 83;

	std::vector<KeyFrame> m_frames;
public:
	Animation(int length, bool loop = true) : m_iterator(0), m_lastFrameTime(0), m_loop(loop), m_playing(false) { m_frames.reserve(length); }
	
	const KeyFrame* Update();

	void PushFrame(int frameIndex, int duration = 1, int socketX = 0, int socketY = 0);
	void Reset();

	const int GetLength() const { return m_frames.size(); }
	const KeyFrame* GetFrame() const { return &m_frames[m_iterator]; }
	const int GetIterator() const { return m_iterator; }
	const int Playing() const { return m_playing; }

	void Play() { m_playing = true; }

	void SetIterator(const int i) { m_iterator = i; }

};