#include "Entity.h"
#include "Camera.h"

void Camera::Update() {

	if (m_mode == CameraMode::FollowTarget && m_target) {
		int playerOffset = -12;
		int camSizeOffset = 8;
		int camOffset = 16;

		m_x = (m_target->X() + playerOffset) / (m_width + camSizeOffset) * (m_width + camSizeOffset) + camOffset;
		m_y = m_target->Y() / m_height * m_height;
	}
}