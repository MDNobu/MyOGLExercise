#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "MyWindowInit.h"
#include "QGLApp.h"
#include "Shader.h"
#include "QGameApp.h"

// �����ڵ������࣬�仯��С

class WindowApplication
{
public:
	static int RunApplication(QGameApp& myGameApp);

	// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
	static void ProcessInput(GLFWwindow* window);

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
	static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	static GLFWwindow* s_Window;

	static const unsigned int WIDTH = 800;
	static const unsigned int HEIGHT = 600;

	static bool s_IsMouseButtonDown;
private:
	static GLFWwindow* CreateWindowAndInit();
	static void UpdateApplication(QGameApp& myGameApp);

	

private:

	//input ��� ���ȷŵ�����֮����ܻ��Ƶ��½���inputģ��
	static bool s_IsFirstTimeMouseIn;

	static float s_MouseLastX;
	static float s_MouseLastY;

	
};

