#pragma once
#include "QGameApp.h"
#include <memory>
#include "QShader.h"
#include "QCamera.h"

class QHelloTexture :
    public QGameApp
{
public:
	QHelloTexture();
	



	virtual void Update(float deltatime) override;

public:
	virtual void RenderScene() override;


	virtual void ShutDown() override;


	void TestOutParam(QCamera& testCamera);
protected:
	virtual void InitAsset() override;



	virtual void InitGameplay() override;

private:
	void SetupVertexData();
	void SetupTextures();

	void SetupCubeVertexData();

	glm::mat4 CustomLookAt(glm::vec3 cameraPosition, glm::vec3 targetPoint, glm::vec3 upVector);

	unsigned int VBO = 0;
	unsigned int VAO = 0;
	unsigned int EBO = 0;
	unsigned int texture = 0;
	unsigned int m_texture2 = 0;

	std::unique_ptr<QShader> m_QShaderProgram;


	glm::vec3 cameraPos{ 0.0, 0.0, 3.0 };
	glm::vec3 targetPoint{0.0, 0.0, 0.0};
	glm::vec3 worldUp{ 0.0, 1.0, 0.0 };
};

