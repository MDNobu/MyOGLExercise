#pragma once
#include "QGameApp.h"
#include <memory>
#include "QShader.h"
#include "QCamera.h"

class QHelloLight :
    public QGameApp
{
public:
	QHelloLight();
	virtual void RenderScene() override;


	virtual void ShutDown() override;


	virtual void Update(float deltatime) override;

protected:
	virtual void InitAsset() override;


	virtual void InitGameplay() override;

private:
	void RenderNonLightObj();
	void RenderLightObj();

	std::unique_ptr<QShader> m_LightShader;
	std::unique_ptr<QShader> m_NonLightShader;

	using GPUResourceHandle = unsigned int;
	GPUResourceHandle m_LightVAO;
	//GPUResourceHandle m_LightEBO;
	GPUResourceHandle m_NonLightVAO;

	// ע�Ȿʾ���� light obj �� nonlight obj����ͬһ��VBO��ֻ��Ҫ��VBO�󶨵�VAO�м��ɣ�
	GPUResourceHandle m_CubeVBO;
	//GPUResourceHandle m_NonLightVBO;
	//GPUResourceHandle m_NonLightEBO;

	void CheckAndPrintGLError();

	glm::vec3 UpdateLightPos(float deltatime);

	glm::vec3 m_LightPos{ 0.0f, 0.0f, 0.0f };
};


