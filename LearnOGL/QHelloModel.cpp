#include "QHelloModel.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "QGLApp.h"
#include "QShader.h"
#include "stb_image.h"
#include "QHelper.h"
#include "WindowApplication.h"
#include "QCamera.h"
#include "QModel.h"

QHelloModel::QHelloModel()
{
	m_Shader = std::make_unique<QShader>();
	m_Model = std::make_unique<QModel>();
}

void QHelloModel::RenderScene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_Shader->Use();
	m_Model->Draw(*m_Shader);
}

void QHelloModel::ShutDown()
{

}

void QHelloModel::Update(float deltatime)
{
	QCamera::GetInstance().Update(deltatime);
	
}

void QHelloModel::InitAsset()
{

	m_Shader->CreateAndSetup("1.model_loading.vs", "1.model_loading.fs");
	m_Model->LoadModel("backpack/backpack.obj");
}

void QHelloModel::InitGameplay()
{
	QCamera::GetInstance().SetToDefaultCamera();


}
