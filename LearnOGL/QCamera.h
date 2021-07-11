#pragma once
#include "QHelper.h"

class QCamera
{
public:
	QCamera();
	QCamera(glm::vec3 cameraPosition, glm::vec3 targetPoint, glm::vec3 upVector);

	void SetEyeAtUp(glm::vec3 eye, glm::vec3 at, glm::vec3 up);

	glm::mat4 GetViewMatrix() const;

	void SetPositionAnRefreshViewMat(glm::vec3 eye);
	glm::vec3 GetPosition() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	static QCamera& GetInstance();

	void AddPitch(float deltaPitch);
	void AddYaw(float deltaYaw);
	void Update(float deltatime);

	void Walk(float delta);
	void Strafe(float delta);

private:
	void RefreshViewMatrix();
public:
	
	//glm::vec3 m_TargetPoint;
	//glm::vec3 m_UpVector;

private:

	glm::vec3 m_CameraPosition;
	//cache 计算结果
	glm::mat4 m_ViewMatrix;
	//这几个向量都是normalize过的
	glm::vec3 m_Forward{ 0.0, 0.0, -1.0 };
	glm::vec3 m_Up{ 0.0, 1.0, 0.0 };
	glm::vec3 m_Right{1.0, 0.0, 0.0};

	float m_Yaw = 0.0f;
	float m_Pitch = -90.0f;

	bool m_IsViewMatDirty = true;

	const static float YAW_INIT ;
	const static float PITCH_INIT;
};



