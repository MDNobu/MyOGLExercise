#pragma once
#include "QGameApp.h"
#include <memory>
#include "QShader.h"

class QHelloTexture :
    public QGameApp
{
public:
	QHelloTexture();
	


public:
	virtual void RenderScene() override;


	virtual void ShutDown() override;

protected:
	virtual void InitAsset() override;


private:
	void SetupVertexData();
	void SetupTextures();

	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;
	unsigned int texture = 0;
	unsigned int m_texture2 = 0;

	std::unique_ptr<QShader> m_QShaderProgram;
};

