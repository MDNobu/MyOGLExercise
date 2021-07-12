#include "QHelloLight.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "QGLApp.h"
#include "Shader.h"
#include "QShader.h"
#include "stb_image.h"
#include "QHelper.h"
#include "WindowApplication.h"
#include "QCamera.h"


QHelloLight::QHelloLight() 
{
	m_LightShader = std::unique_ptr<QShader>(new QShader());
	m_NonLightShader = std::unique_ptr<QShader>(new QShader());
}

void QHelloLight::RenderScene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

#pragma region RenderLightObj
	RenderLightObj();
#pragma endregion RenderLightObj
	
#pragma region RenderNonlightObj
	RenderNonLightObj();
#pragma endregion RenderNonlightObj
	
}

void QHelloLight::ShutDown()
{

	//glDeleteVertexArrays(1, &m_LightVAO);
	//glDeleteVertexArrays(1, &m_NonLightVAO);
	//glDeleteBuffers(1, &m_CubeVBO);
	//glDeleteBuffers(1, &EBO);
}

void QHelloLight::Update(float deltatime)
{
	QCamera::GetInstance().Update(deltatime);
}

void QHelloLight::InitAsset()
{
#pragma region 准备shaderPrograms
	m_LightShader->CreateAndSetup("1.light_cube.vs", "1.light_cube.fs");
	m_NonLightShader->CreateAndSetup("1.colors.vs", "1.colors.fs");
	CheckAndPrintGLError();
#pragma endregion 准备shaderPrograms

	// TODO 添加法线数据，并打开相应attribute

	// Cube 顶点数据， light 和non light都用了cube数据，包括同一个VBO
	float cubeVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

#pragma region SetupLightResources


	glGenVertexArrays(1, &m_LightVAO);
	glGenBuffers(1, &m_CubeVBO);
	CheckAndPrintGLError();
	glBindVertexArray(m_LightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	CheckAndPrintGLError();
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
	CheckAndPrintGLError();
#pragma endregion SetupLightResources

#pragma region SetupNonlightResources
	glGenVertexArrays(1, &m_NonLightVAO);
	//glGenBuffers(1, &m_NonLightVBO);

	glBindVertexArray(m_NonLightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
#pragma endregion SetupNonlightResources



}

void QHelloLight::InitGameplay()
{
	QCamera::GetInstance().SetToDefaultCamera();
}


void QHelloLight::RenderNonLightObj()
{
	glBindVertexArray(m_NonLightVAO);
	m_NonLightShader->Use();
	using namespace glm;
	mat4 identityMat = mat4(1.0f);
	mat4 model = identityMat;
	mat4 view = QCamera::GetInstance().GetViewMatrix();
	mat4 proj = QCamera::GetInstance().GetProjMatrix();
	mat4 mvp = proj * view * model;
	m_NonLightShader->SetMatrix("model", model);
	m_NonLightShader->SetMatrix("mvp", mvp);

	// TODO 光照设置常量先直接放在这里，后续添加light对象之后移过去
#pragma region lightSettings
	const glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	const glm::vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	const float shineness = 3.0f;
	const float ambientIntensity = 0.5f;
#pragma endregion lightSettings
	m_NonLightShader->SetVec3("objectColor", objectColor);
	m_NonLightShader->SetVec3("lightColor", lightColor);
	// 这里先假设 lightDir是这样
	m_NonLightShader->SetVec3("uLightPosWS", lightPos);
	m_NonLightShader->SetFloat("shineness", shineness);
	m_NonLightShader->SetFloat("ambientIntensity", ambientIntensity);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void QHelloLight::RenderLightObj()
{
	const glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


	glBindVertexArray(m_LightVAO);
	m_LightShader->Use();
	using namespace glm;
	mat4 identityMat = mat4(1.0f);
	mat4 model = glm::translate(identityMat, lightPos);

	mat4 view = QCamera::GetInstance().GetViewMatrix();
	//mat4 projMat = QCamera::GetInstance()
	mat4 proj = QCamera::GetInstance().GetProjMatrix();
	mat4 mvp = proj * view * model;
	m_LightShader->SetMatrix("mvp", mvp);

	CheckAndPrintGLError();

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	
	CheckAndPrintGLError();
}

void QHelloLight::CheckAndPrintGLError()
{
	unsigned int glErrorCode = glGetError();
	if (glErrorCode)
	{
		std::cout << "GLError = " << glGetError() << std::endl;
	}
}
