#pragma once
#include "QHelper.h"

class QCamera
{
public:
	QCamera();
	QCamera(glm::vec3 cameraPosition, glm::vec3 targetPoint, glm::vec3 upVector);

	void SetEyeAtUp(glm::vec3 eye, glm::vec3 at, glm::vec3 up);

	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjMatrix() const;

	void SetPositionAnRefreshViewMat(glm::vec3 eye);
	glm::vec3 GetPosition() const;

	glm::vec3 GetForward() const;
	glm::vec3 GetRight() const;
	glm::vec3 GetUp() const;

	static QCamera& GetInstance();

	void AddPitch(float deltaPitch);
	void AddYaw(float deltaYaw);
	void Update(float deltatime);

	virtual void Walk(float delta);
	virtual void Strafe(float delta);

	void SetToDefaultCamera();

	void RefreshViewMatrix();
private:
	
public:
	
	//glm::vec3 m_TargetPoint;
	//glm::vec3 m_UpVector;

protected:
	glm::vec3 m_CameraPosition{ 0.0, 0.0, 3.0 };
	//cache 计算结果
	glm::mat4 m_ViewMatrix = glm::mat4(1.0);
	//这几个向量都是normalize过的
	glm::vec3 m_Forward{ 0.0, 0.0, -1.0 };
	glm::vec3 m_Up{ 0.0, 1.0, 0.0 };
	glm::vec3 m_Right{ 1.0, 0.0, 0.0 };

	glm::mat4 m_ProjMatrix = glm::mat4(1.0);

	float m_Yaw = 0.0f;
	float m_Pitch = -90.0f;

	bool m_IsViewMatDirty = true;
private:



	const static float YAW_INIT ;
	const static float PITCH_INIT;

	static const unsigned int WIDTH = 800;
	static const unsigned int HEIGHT = 600;
};



