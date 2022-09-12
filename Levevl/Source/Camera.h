#pragma once

class Entity;

enum class CameraMode {
	FollowTarget,
	Free
};

class Camera {
public:
	int m_x, m_y;
	int m_width, m_height;
	Entity* m_target;
	CameraMode m_mode;
private:
	//
public:
	Camera(int x, int y, int width, int height) : m_x(x), m_y(y), m_width(width), m_height(height), m_mode(CameraMode::FollowTarget), m_target(nullptr) {}

	void Update();
};