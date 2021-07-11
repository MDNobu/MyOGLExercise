#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "MyWindowInit.h"
#include "QGLApp.h"
#include "Shader.h"
#include "WindowApplication.h"
#include "QGameApp.h"
#include "QHelloTriangle.h"
#include "QHelloTexture.h"
#include "QHelloLight.h"


int main()
{

	//QHelloTriangle myGameApp;
	//QHelloTexture myGameApp;
	QHelloLight myGameApp;
	return WindowApplication::RunApplication(myGameApp);
}



