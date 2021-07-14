#pragma once
#include "QGameApp.h"
#include <memory>
#include "QShader.h"
#include "QCamera.h"

class QAdanceOGL :
    public QGameApp
{


public:
	virtual void RenderScene() override;

	






	virtual void ShutDown() override;


	virtual void Update(float deltatime) override;

protected:
	virtual void InitAsset() override;


	virtual void InitGameplay() override;

private:
	void DrawPlane(const QShader& shader) const;
	void Draw2Cubes(const QShader& shader, const glm::mat4& scaleMat) const;
private:
	QShader m_NormalShader;
	QShader m_OutlineShader;

	GPUResourceHandle m_CubeVAO = 0;
	GPUResourceHandle m_CubeVBO = 0;
	GPUResourceHandle m_PlaneVAO = 0;
	GPUResourceHandle m_PlaneVBO = 0;

	GPUResourceHandle m_CubeTexture = 0;
	GPUResourceHandle m_PlaneTexture = 0;
};

