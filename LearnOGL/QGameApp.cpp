#include "QGameApp.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "MyWindowInit.h"
#include "QGLApp.h"
#include "Shader.h"

void QGameApp::Init()
{
	//throw std::logic_error("The method or operation is not implemented.");

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw std::runtime_error("glad init failed");
	}

	InitPipeline();
	InitAsset();
}

void QGameApp::RenderScene()
{
	//throw std::logic_error("The method or operation is not implemented.");


}

void QGameApp::ShutDown()
{

}

void QGameApp::InitPipeline()
{
	
}

void QGameApp::InitAsset()
{

	
}
