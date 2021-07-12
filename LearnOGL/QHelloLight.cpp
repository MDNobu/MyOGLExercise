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
	UpdateLightPos(deltatime);
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
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
#pragma endregion SetupNonlightResources

	SetupTextureResources();
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
	m_NonLightShader->SetVec3("uLightPosWS", m_LightPos);
	
	m_NonLightShader->SetFloat("ambientIntensity", ambientIntensity);

	m_NonLightShader->SetFloat("cubeMaterial.shineness", shineness);

	//m_NonLightShader->SetS
	m_NonLightShader->SetInt("cubeMaterial.baseColorMap", 0);
	m_NonLightShader->SetInt("cubeMaterial.specularMap", 1);
	m_NonLightShader->SetInt("cubeMaterial.emissiveMap", 2);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

void QHelloLight::RenderLightObj()
{
	const glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
	//glm::vec3 newLightPos = UpdateLightPos(1.0f);

	glBindVertexArray(m_LightVAO);
	m_LightShader->Use();
	using namespace glm;
	mat4 identityMat = mat4(1.0f);
	mat4 model = glm::translate(identityMat, m_LightPos);

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

glm::vec3 QHelloLight::UpdateLightPos(float deltatime)
{
	using namespace glm;
	const glm::vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	const float roateSpeed = 1.0f;
	float rotateAngle = glfwGetTime() * roateSpeed;
	 
	mat4 roateMat = glm::rotate(glm::identity<mat4>(), rotateAngle, glm::vec3(0.0f, 0.0f, - 1.0f));
	vec3 resLightPos = vec3( roateMat * vec4(lightPos, 1.0f) );
	m_LightPos = resLightPos;
	return resLightPos;
}

void QHelloLight::SetupTextureResources()
{
	// setup base map
	{
		glGenTextures(1, &m_BaseMap);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_BaseMap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		//unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	
	CheckAndPrintGLError();

	// Setup specular map
	{
		glGenTextures(1, &m_SpecularMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_SpecularMap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		//unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("Textures/container2_specular.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	CheckAndPrintGLError();

	// Setup emissive map
	{
		glGenTextures(1, &m_EmissiveMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_EmissiveMap);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		//unsigned char* data = stbi_load("Textures/container.jpg", &width, &height, &nrChannels, 0);
		stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load("Textures/matrix.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}

	CheckAndPrintGLError();
}
