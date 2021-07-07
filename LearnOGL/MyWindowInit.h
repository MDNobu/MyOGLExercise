#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace MyWindowInit
{

	//typedef void (*GLFWframebuffersizefun)(GLFWwindow*, int, int);
	using namespace std;
	void QoFramebufferResizeCB(GLFWwindow* window, int newWidth, int newHeight)
	{
		//glViewport(0, 0, newWidth, newHeight);

		cout << "QoFramebufferResizeCB is called" << endl;
	}


	void QoProcessInput(GLFWwindow* glWindow)
	{
		if (GLFW_PRESS == glfwGetKey(glWindow, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(glWindow, true);
		}
	}

	void QoDraw()
	{

		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		//glClearColor(0, 0, 0, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		//gl
		//glClear()
		//throw std::logic_error("The method or operation is not implemented.");
	}

	int StartRun()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		GLFWwindow* glWindow = glfwCreateWindow(800, 600, "LearnOGL", nullptr, nullptr);
		if (nullptr == glWindow)
		{

			std::cout << "glfw init failed" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(glWindow);
		//glfwsetcon

		 //manages function pointers for OpenGL so we want to initialize GLAD before we call any OpenGL functions
		bool initGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		if (!initGlad)
		{
			return -1;
		}

		glfwSetFramebufferSizeCallback(glWindow, QoFramebufferResizeCB);

#pragma region GameLoop
		while (!glfwWindowShouldClose(glWindow))
		{
			QoProcessInput(glWindow);

			QoDraw();

			glfwPollEvents();
			glfwSwapBuffers(glWindow);
		}
#pragma endregion
		glfwTerminate();
	}
}
