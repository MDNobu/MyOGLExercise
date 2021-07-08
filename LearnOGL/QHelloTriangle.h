#pragma once
#include "QGameApp.h"
#include <memory>
#include "QShader.h"

class Shader;
class QShader;
class QHelloTriangle :
    public QGameApp
{

public:
	QHelloTriangle();
	virtual void RenderScene() override;


	virtual void ShutDown() override;

protected:
	virtual void InitAsset() override;

private:
	//unsigned int shaderProgram = 0;
	unsigned int m_VBO = 0;
	unsigned int m_VAO = 0;

	std::unique_ptr<QShader> m_QShaderProgram;
	//QShader* m_QShaderProgram;

	//Shader* m_Shader;
};

