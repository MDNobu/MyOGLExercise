#pragma once
#include "QGameApp.h"
#include <memory>
#include "QShader.h"
#include "QCamera.h"
#include "QCustomizeRenderable.h"
#include "QModel.h"
//#include "model.h"
//#include "Shader.h"

class QAdanceOGL :
    public QGameApp
{


public:
	QAdanceOGL();

	virtual void RenderScene() override;


	virtual void ShutDown() override;


	virtual void Update(float deltatime) override;

protected:
	virtual void InitAsset() override;


	virtual void InitGameplay() override;

	
private:

	void InitOldAssets();
	void InitCubemapSceneAsset();
	void DrawPlane(const QShader& shader) const;
	void Draw2Cubes(const QShader& shader, const glm::mat4& scaleMat) const;

	void DrawOutlineCubes();
	void DrawTransparentPlanes(const QShader& shader);
	void InitCustomFramebuffer();
	void RenderWithCustomFrameBuffer();
	void DrawSkybox(const QShader& shader);
	void DrawCubeInSkybox(const QShader& shader);

	void DrawWithCubemaps();

	void InitProcedureHouseAsset();
private:
	QShader m_NormalShader;
	QShader m_OutlineShader;
	QShader m_PPShader;
	QShader m_SkyboxShader;
	QShader m_CenterCubeShader;
	QShader m_ProcedureHouseShader;

	GPUResourceHandle m_CubeVAO = 0;
	GPUResourceHandle m_CubeVBO = 0;
	GPUResourceHandle m_PlaneVAO = 0;
	GPUResourceHandle m_PlaneVBO = 0;
	GPUResourceHandle m_TransPlaneVAO = 0;
	GPUResourceHandle m_TransPlaneVBO = 0;

	GPUResourceHandle m_CubeTexture = 0;
	GPUResourceHandle m_PlaneTexture = 0;
	GPUResourceHandle m_TranPlaneTexture = 0;

	GPUResourceHandle m_QuadVAO = 0;
	GPUResourceHandle m_QuadVBO = 0;

	GPUResourceHandle m_CustomFramebuffer = 0;
	GPUResourceHandle m_RenderTargetTexture = 0;

	GPUResourceHandle m_Cubemap = 0;
	

	std::unique_ptr<QCustomizeRenderable> m_CenterCube = 0;
	std::unique_ptr<QCustomizeRenderable> m_Skybox = 0;

	std::unique_ptr<QCustomizeRenderable> m_ProcedureFlatHouse = 0;

	std::unique_ptr<QModel> m_PackModel = 0;
	QShader m_PackShader;

	//std::unique_ptr<Model> m_PackModel2 = 0;
	//std::unique_ptr<Shader> m_PackShader2 = 0;
};

