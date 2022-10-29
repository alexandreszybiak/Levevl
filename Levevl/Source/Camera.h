#pragma once
#include "Utilities.h"

class Entity;
class Level;

enum class CameraMode {
	FollowTarget,
	Free
};

class Camera {
public:
	int m_x, m_y;
	Vector2 m_oldPos;
	int m_width, m_height;
	Entity* m_target;
	CameraMode m_mode;
	Level* m_levelRef;
private:
	//
public:
	Camera(int x, int y, int width, int height) : m_x(x), m_y(y), m_oldPos({ x,y }), m_width(width), m_height(height), m_mode(CameraMode::Free), m_target(nullptr), m_levelRef(nullptr) {}

	void Update();
};