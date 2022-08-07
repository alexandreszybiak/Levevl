#pragma once

class KeyFrame {
private:
	int m_frameIndex;
	int m_duration;
	int m_socketX;
	int m_socketY;

public:
	// Creates a KeyFrame with a frame index and a duration in frames.
	KeyFrame(int frameIndex, int duration, int socketX = 0, int socketY = 0) : m_frameIndex(frameIndex), m_duration(duration), m_socketX(socketX), m_socketY(socketY) {}

	const int GetFrameIndex() const { return m_frameIndex; }
	const int GetDuration() const { return m_duration; }
	const int GetSocketX() const { return m_socketX; }
	const int GetSocketY() const { return m_socketY; }
};