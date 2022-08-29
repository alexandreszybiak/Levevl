#pragma once

class KeyFrame {
private:
	int m_frameIndex;
	int m_duration;

public:
	// Creates a KeyFrame with a frame index and a duration in frames.
	KeyFrame(int frameIndex, int duration) : m_frameIndex(frameIndex), m_duration(duration) {}

	const int GetFrameIndex() const { return m_frameIndex; }
	const int GetDuration() const { return m_duration; }
};