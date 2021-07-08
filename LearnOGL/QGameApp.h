#pragma once

//#include "WindowApplication.h"

class QGameApp
{
public:
	void Init();
	void RenderScene();
	void ShutDown();
private:
	void InitPipeline();

	unsigned int shaderProgram;
	unsigned int VBO;
	unsigned int VAO;
};

