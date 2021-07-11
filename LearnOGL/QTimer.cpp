#include "QTimer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

QTimer& QTimer::GetInstance()
{
	static QTimer timer;
	return timer;
}

void QTimer::Update()
{
	float currentTime = glfwGetTime();
	// 正常来说这里应该不会发生溢出之类的问题
	m_DeltatTime = currentTime - m_LastFrame;
}

float QTimer::GetDeltaTime()
{
	return m_DeltatTime;
}
