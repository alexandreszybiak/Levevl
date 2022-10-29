#include <iostream>
#include "Entity.h"
#include "Level.h"
#include "Camera.h"

void Camera::Update() {

	if (m_mode == CameraMode::FollowTarget && m_target) {
		int playerOffset = -12;
		int camSizeOffset = 8;
		int camOffset = 16;

		m_x = (m_target->X() + playerOffset) / (m_width + camSizeOffset) * (m_width + camSizeOffset) + camOffset;
		m_y = m_target->Y() / m_height * m_height;

		if (Vector2(m_x, m_y) != m_oldPos) {
			std::cout << "New room entered" << std::endl;
			m_levelRef->ActivateTiles();
			
		}

		m_oldPos.x = m_x;
		m_oldPos.y = m_y;
	}
}