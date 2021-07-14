#include "QGameApp.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "MyWindowInit.h"
//#include "QGLApp.h"
#include "QCamera.h"

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

	InitGameplay();
	InitAsset();
}

void QGameApp::RenderScene()
{
	//throw std::logic_error("The method or operation is not implemented.");
	

}

void QGameApp::ShutDown()
{

}

void QGameApp::Update(float deltatime)
{
	QCamera::GetInstance().Update(deltatime);
}

void QGameApp::InitGameplay()
{
	QCamera::GetInstance().SetToDefaultCamera();
}

void QGameApp::InitAsset()
{

	
}
