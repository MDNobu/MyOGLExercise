#include "QCamera.h"
#include "QFPSCamera.h"

const float QCamera::YAW_INIT = 0.0f;
const float QCamera::PITCH_INIT = -90.0f;

QCamera::QCamera() 
{

}



QCamera::QCamera(glm::vec3 cameraPosition, glm::vec3 targetPoint, glm::vec3 worldUpVector) 
{
	SetEyeAtUp(cameraPosition, targetPoint, worldUpVector);
}

void QCamera::SetEyeAtUp(glm::vec3 eye, glm::vec3 at, glm::vec3 worldUp)
{
	m_CameraPosition = eye;
	using  namespace glm;


	//vec<3, T, Q> const f(normalize(center - eye));
//vec<3, T, Q> const s(normalize(cross(up, f)));
//vec<3, T, Q> const u(cross(f, s));
	m_Forward = normalize(at - eye);
	m_Right = normalize(cross(m_Forward, worldUp));
	m_Up = cross(m_Right, m_Forward);
	//RefreshViewMatrix();
	//m_ViewMatrix = glm::lookAt(m_CameraPosition, m_TargetPoint, m_UpVector);
	//RefreshViewMatrix();
	//m_ViewMatrix = glm::lookAt(m_CameraPosition, m_TargetPoint, m_UpVector);
	m_IsViewMatDirty = true;
}

glm::mat4 QCamera::GetViewMatrix() const
{
	assert(!m_IsViewMatDirty);
	return m_ViewMatrix;
}

glm::mat4 QCamera::GetProjMatrix() const
{

	return m_ProjMatrix;
}

void QCamera::SetPositionAnRefreshViewMat(glm::vec3 eye)
{
	m_CameraPosition = eye;
	//m_TargetPoint = m_CameraPosition + m_Forward * 5.0f;
}

glm::vec3 QCamera::GetPosition() const
{
	return m_CameraPosition;
}

glm::vec3 QCamera::GetForward() const
{
	return m_Forward;
}

glm::vec3 QCamera::GetRight() const
{
	return m_Right;
}

glm::vec3 QCamera::GetUp() const
{
	return m_Up;
}

QCamera& QCamera::GetInstance()
{
	static QCamera camera;
	//static QFPSCamera camera;
	return camera;
}

void QCamera::AddPitch(float deltaPitch)
{
	if (deltaPitch > 89.0f)
	{
		deltaPitch = 89.0f;
	}
	if (deltaPitch < -89.0f)
	{
		deltaPitch = -89.0f;
	}

	// 关于right 方向旋转pitch度, 此时right不变，rebuild 另外两个
	using namespace glm;
	mat4 identityMat = glm::mat4(1.0);
	mat4 rotationMat = glm::rotate(identityMat, -glm::radians(deltaPitch), m_Right);

	//glm::eulerAngles()
	//vec4 forward = ;
	m_Forward = vec3(rotationMat * vec4(m_Forward, 0.0f));
	m_Up = vec3(rotationMat * vec4(m_Up, 0.0f));

	m_IsViewMatDirty = true;
}

void QCamera::AddYaw(float deltaYaw)
{
	// 关于world space up 转deltaYaw
	using namespace glm;
	glm::mat4 identityMat = glm::mat4(1.0);
	glm::mat4 rotationMat = glm::rotate(identityMat, -glm::radians(deltaYaw), glm::vec3(0.0f, 1.0f, 0.0f));

	m_Forward = vec3(rotationMat * vec4(m_Forward, 0.0f));
	m_Up = vec3(rotationMat * vec4(m_Up, 0.0f));
	m_Right = vec3(rotationMat * vec4(m_Right, 0.0f));
	m_IsViewMatDirty = true;
}

void QCamera::Update(float deltatime)
{
	RefreshViewMatrix();
}

void QCamera::Walk(float delta)
{
	m_CameraPosition += m_Forward * delta;
	m_IsViewMatDirty = true;
}

void QCamera::Strafe(float delta)
{
	m_CameraPosition += m_Right * delta;
	m_IsViewMatDirty = true;
}

void QCamera::SetToDefaultCamera()
{

	glm::vec3 cameraPos{ 0.0, 0.0, 3.0 };
	glm::vec3 targetPoint{ 0.0, 0.0, 0.0 };
	glm::vec3 worldUp{ 0.0, 1.0, 0.0 };

	SetEyeAtUp(cameraPos, targetPoint, worldUp);

	m_ProjMatrix = glm::perspective(glm::radians(45.0f), (float)QCamera::WIDTH / QCamera::HEIGHT, 0.1f, 100.0f);
}

void QCamera::RefreshViewMatrix()
{
	if (m_IsViewMatDirty)
	{
		// TODO  这里需要要更正，移动的时候targetPoint是可能发生变化的直接用lookAt计算不对
		using namespace glm;
		
		//m_Forward = glm::normalize(m_TargetPoint - m_CameraPosition);
		//m_Right = normalize(cross(m_Forward, m_UpVector));
		//m_Up = cross(m_Right, m_Forward);
		//vec<3, T, Q> const f(normalize(center - eye));
		//vec<3, T, Q> const s(normalize(cross(up, f)));
		//vec<3, T, Q> const u(cross(f, s));
		m_Forward = normalize(m_Forward);
		 m_Right = normalize(cross(m_Forward, m_Up));
		 m_Up = cross(m_Right, m_Forward);

		//glm::lookAt();
		vec3 xAsix = m_Right;
		vec3 yAxis = m_Up;
		vec3 zAxis = -m_Forward;
		vec3 eye = m_CameraPosition;


		mat4 Result = glm::mat4(1.0f);
		Result[0][0] = xAsix.x;
		Result[1][0] = xAsix.y;
		Result[2][0] = xAsix.z;
		Result[0][1] = yAxis.x;
		Result[1][1] = yAxis.y;
		Result[2][1] = yAxis.z;
		Result[0][2] = zAxis.x;
		Result[1][2] = zAxis.y;
		Result[2][2] = zAxis.z;
		Result[3][0] = -dot(xAsix, eye);
		Result[3][1] = -dot(yAxis, eye);
		Result[3][2] = -dot(zAxis, eye);
		//return Result;
		m_ViewMatrix = Result;



		//更新 yaw pitch roll 
		//glm::eulerAngles()

		m_IsViewMatDirty = false;
	}

}

void QCamera::MoveUp(float deltaPos)
{
	m_CameraPosition += m_Up * deltaPos;
	m_IsViewMatDirty = true;
}
