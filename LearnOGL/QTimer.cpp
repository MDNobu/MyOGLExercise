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
	// ������˵����Ӧ�ò��ᷢ�����֮�������
	m_DeltatTime = currentTime - m_LastFrame;
}

float QTimer::GetDeltaTime()
{
	return m_DeltatTime;
}
