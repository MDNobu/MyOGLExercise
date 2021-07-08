#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "MyWindowInit.h"
#include "QGLApp.h"
#include "Shader.h"
#include "WindowApplication.h"
#include "QGameApp.h"
#include "QGameApp.h"

// �������ڵ������࣬�仯��С

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
};
