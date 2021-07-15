#include "QAdanceOGL.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "MyWindowInit.h"
#include "QGLApp.h"
#include "QShader.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "QHelper.h"
#include "QCamera.h"
#include <vector>
#include <algorithm>
#include <utility>


QAdanceOGL::QAdanceOGL()
{

}

void QAdanceOGL::RenderScene()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);

	DrawSkybox(m_SkyboxShader);

	DrawCubeInSkybox(m_CenterCubeShader);
}

void QAdanceOGL::Draw2Cubes(const QShader& shader, const glm::mat4& scaleMat = glm::mat4(1.0)) const
{
	shader.Use();

	using namespace glm;
	glActiveTexture(GL_TEXTURE0);
	// draw 1st cube
	glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
	
	
	glm::mat4 view = QCamera::GetInstance().GetViewMatrix();
	glm::mat4 projection = QCamera::GetInstance().GetProjMatrix();
	shader.SetMatrix("view", view);
	shader.SetMatrix("projection", projection);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	model =  model * scaleMat;
	shader.SetMatrix("model", model);
	glBindVertexArray(m_CubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// draw 2nd cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
	model =  model * scaleMat;
	shader.SetMatrix("model", model);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	//glBindVertexArray(0);
}

void QAdanceOGL::DrawOutlineCubes()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClearDepth(1.0);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//glDisable(GL_DEPTH_TEST);


	glStencilMask(0x00);
	DrawPlane(m_NormalShader);

	glEnable(GL_STENCIL_TEST);
	glm::mat4 scaleMat = glm::mat4(1.0f);

	// stencil绘制一遍
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilMask(0xFF);
	Draw2Cubes(m_NormalShader, scaleMat);

	const float scale = 1.1f;
	scaleMat = glm::scale(scaleMat, glm::vec3(scale, scale, scale));
	// outline 绘制一遍
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	Draw2Cubes(m_OutlineShader, scaleMat);
	glStencilFunc(GL_ALWAYS, 0, 0xFF);
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
	glDisable(GL_STENCIL_TEST);
	//glStencilMask(0x00);
}

void QAdanceOGL::DrawTransparentPlanes(const QShader& shader)
{
	shader.Use();
	
	glBindTexture(GL_TEXTURE_2D, m_TranPlaneTexture);

	using namespace std;
	// transparent window locations
   // --------------------------------
	vector<glm::vec3> windows
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(1.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	shader.SetupCameraUniforms(QCamera::GetInstance());

	glm::vec3 cameraPos = QCamera::GetInstance().GetPosition();
	auto cmp = [cameraPos](glm::vec3 lhs, glm::vec3 rhs) ->  bool{
		return glm::length(lhs - cameraPos) < glm::length(rhs - cameraPos);
	};
	std::sort(windows.begin(), windows.end(), cmp);

	//glDisable(GL_DEPTH_TEST);
	glDepthMask(0x00);
	glBindVertexArray(m_TransPlaneVAO);
	for (const auto& element : windows)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, element);
		shader.SetMatrix("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindVertexArray(0);
	glDepthMask(0xFF);
	//glEnable(GL_DEPTH_TEST);
}

void QAdanceOGL::InitCustomFramebuffer()
{
	glGenFramebuffers(1, &m_CustomFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_CustomFramebuffer);
	
	glGenTextures(1, &m_RenderTargetTexture);
	glBindTexture(GL_TEXTURE_2D, m_RenderTargetTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTargetTexture, 0);

	GPUResourceHandle rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	QHelper::CheckAndPrintGLError();
}

void QAdanceOGL::RenderWithCustomFrameBuffer()
{
	QHelper::CheckAndPrintGLError();

	glBindFramebuffer(GL_FRAMEBUFFER, m_CustomFramebuffer);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawPlane(m_NormalShader);
	Draw2Cubes(m_NormalShader, glm::mat4(1.0));

	glEnable(GL_BLEND);
	DrawTransparentPlanes(m_NormalShader);
	glDisable(GL_BLEND);

	// switch to default fb
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//clear default FB
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	m_PPShader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	m_PPShader.SetupCameraUniforms(QCamera::GetInstance());
	m_PPShader.SetMatrix("model", model);

	glBindTexture(GL_TEXTURE_2D, m_RenderTargetTexture);
	glBindVertexArray(m_QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);

	QHelper::CheckAndPrintGLError();
}

void QAdanceOGL::DrawSkybox(const QShader& shader)
{
	shader.Use();
	shader.SetupCameraUniforms(QCamera::GetInstance());
	shader.SetMatrix("view",
		glm::mat4(glm::mat3(QCamera::GetInstance().GetViewMatrix()) ));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Cubemap);
	glDepthFunc(GL_LEQUAL);

	m_Skybox->Draw(shader);

	glDepthFunc(GL_LESS);
}

void QAdanceOGL::DrawCubeInSkybox(const QShader& shader)
{
	shader.Use();
	shader.SetupCameraUniforms(QCamera::GetInstance());
	shader.SetVec3("colorTint", glm::vec3(0.0f, 1.0f, 0.0f));
	shader.SetVec3("cameraPosition", QCamera::GetInstance().GetPosition());

	m_CenterCube->Draw(shader);

}

void QAdanceOGL::InitOldAssets()
{
	m_NormalShader.CreateAndSetup("1.1.depth_testing.vs", "1.1.depth_testing.fs");
	m_OutlineShader.CreateAndSetup("1.1.depth_testing.vs", "stencil_testing.fs");
	m_PPShader.CreateAndSetup("5.1.framebuffers_screen.vs", "5.1.framebuffers_screen.fs");

#pragma region SetupVertexDatas
	// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
	float cubeVertices[] = {
		// positions          // texture Coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};
	// #TODO 这里复制不是什么好的做法，先这样做，有机会重构

	// setup cube vao/vbo
	{
		glGenVertexArrays(1, &m_CubeVAO);
		glGenBuffers(1, &m_CubeVBO);

		glBindVertexArray(m_CubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	// setup plane vao/vbo
	{
		glGenVertexArrays(1, &m_PlaneVAO);
		glGenBuffers(1, &m_PlaneVBO);

		glBindVertexArray(m_PlaneVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}


	{
		glGenVertexArrays(1, &m_TransPlaneVAO);
		glGenBuffers(1, &m_TransPlaneVBO);

		glBindVertexArray(m_TransPlaneVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_TransPlaneVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	   // positions   // texCoords
	   -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  0.0f, 1.0f, 0.0f,

	   -1.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  0.0f,1.0f, 0.0f,
		1.0f,  1.0f,  0.0f, 1.0f, 1.0f
	};

	QHelper::GenerateStandardVABO(quadVertices, sizeof(quadVertices) / sizeof(float), &m_QuadVAO, &m_QuadVBO);


#pragma endregion SetupVertexDatas

#pragma region LoadTextures
	m_CubeTexture = QHelper::LoadTextureAndUpload2GPU("textures/container.jpg");
	m_PlaneTexture = QHelper::LoadTextureAndUpload2GPU("textures/wall.jpg");
	m_TranPlaneTexture = QHelper::LoadTextureAndUpload2GPU("textures/window.png");
	
#pragma endregion LoadTextures


	InitCustomFramebuffer();
}

void QAdanceOGL::DrawPlane(const QShader& shader) const
{
	//draw plane
	shader.Use();
	glBindTexture(GL_TEXTURE_2D, m_PlaneTexture);
	shader.SetMatrix("model", glm::mat4(1.0f));
	glBindVertexArray(m_PlaneVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void QAdanceOGL::ShutDown()
{

}

void QAdanceOGL::Update(float deltatime)
{
	QGameApp::Update(deltatime);
}



void QAdanceOGL::InitAsset()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
// ------------------------------------------------------------------
	float cubeVertices[] = {
		// positions          // normals
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

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
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	m_CenterCube = std::make_unique<QCustomizeRenderable>(cubeVertices,
		sizeof(cubeVertices)/sizeof(float), QCustomizeRenderable::InitVertexDataType::POS_NORMAL);

	m_Skybox = std::make_unique<QCustomizeRenderable>(skyboxVertices,
		sizeof(skyboxVertices) / sizeof(float), QCustomizeRenderable::InitVertexDataType::POS_ONLY);

	m_SkyboxShader.CreateAndSetup("skybox.vs","skybox.fs");
	m_CenterCubeShader.CreateAndSetup("envmap_test.vs", "envmap_test.fs");

	// load textures
	// -------------
	std::vector<std::string> faces
	{
		"textures/skybox/right.jpg",
		"textures/skybox/left.jpg",
		"textures/skybox/top.jpg",
		"textures/skybox/bottom.jpg",
		"textures/skybox/front.jpg",
		"textures/skybox/back.jpg",
	};
	m_Cubemap = QHelper::LoadCubemapAndUpload2GPU(faces);
}

void QAdanceOGL::InitGameplay()
{
	//throw std::logic_error("The method or operation is not implemented.");
	QGameApp::InitGameplay();
}
