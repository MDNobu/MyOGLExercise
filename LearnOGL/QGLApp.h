#pragma once



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

extern const char* g_VertexShaderSource;
extern const char* g_FragmentShaderSource;

class QGLApp
{
public:
	QGLApp();
	
	bool Init();

	int Run();


public:
	static void QGLAppResizeCallBack(GLFWwindow*, int width, int height);

private:
	bool InitWindow();
	void ProcessInput(GLFWwindow* window);


	unsigned int CreateAndCompileShaderWithCheck(GLenum shaderType, const char *shaderContent);

private:
	GLFWwindow* m_glWindow;

};

