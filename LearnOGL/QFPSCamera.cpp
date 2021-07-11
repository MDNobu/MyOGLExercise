#include "QFPSCamera.h"

void QFPSCamera::Walk(float delta)
{
	using namespace glm;
	vec3 forwardXZ = glm::normalize(vec3(m_Forward.x, 0.0, m_Forward.z));
	
	m_CameraPosition += glm::dot(forwardXZ, m_Forward) * delta;
	m_IsViewMatDirty = true;

	//RefreshViewMatrix();
}

void QFPSCamera::Strafe(float delta)
{
	QCamera::Strafe(delta);
}
