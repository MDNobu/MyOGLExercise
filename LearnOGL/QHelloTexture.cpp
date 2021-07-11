#include "QHelloTexture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
//#include "MyWindowInit.h"
#include "QGLApp.h"
#include "Shader.h"
#include "QShader.h"
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "WindowApplication.h"
#include "QCamera.h"

QHelloTexture::QHelloTexture() : m_QShaderProgram(nullptr)
{
	m_QShaderProgram = std::unique_ptr<QShader>(new QShader());

	cameraPos = glm::vec3(0.0, 0.0, 3.0);
	targetPoint = glm::vec3(0.0, 0.0, 0.0);
	worldUp =  glm::vec3(0.0, 1.0, 0.0);
}

void QHelloTexture::Update(float deltatime)
{
	QCamera::GetInstance().Update(deltatime);

}

void QHelloTexture::RenderScene()
{
	//throw std::logic_error("The method or operation is not implemented.");
	// render
	   // ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(VAO);
	// bind Texture
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, m_texture2);
	// render container
	
	 
	// 更新constant buffers
	m_QShaderProgram->Use();
	m_QShaderProgram->SetInt("texture1", 0);
	m_QShaderProgram->SetInt("texture2", 1);

	//m_QShaderProgram
	
	//unsigned int uniformLocal = glGetUniformLocation(m_QShaderProgram->m_ShaderProgramID, "colorTint");
	//glUniform4f(uniformLocal, 1.0f, 0.0f, 0.0f, 1.0f);


	static glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	using namespace glm;
	// setting view & projection matrix
	glm::mat4 identityMat = glm::mat4(1.0f);

	
	
	//mat4 view = glm::lookAt(vec3(cameraPosition), vec3(0, 0, 0), vec3(0.0f, 1.0f, 0.0f));
	//QCamera::GetInstance().SetEyeAtUp(cameraPos, targetPoint, worldUp);
	QCamera& camera = QCamera::GetInstance();
	mat4 view = std::move<mat4>(QCamera::GetInstance().GetViewMatrix());
	vec3 up = camera.GetUp();
	vec3 right = camera.GetRight();
	vec3 forward = camera.GetForward();
	//view = CustomLookAt(cameraPos, targetPoint, worldUp);
	//view = glm::lookAt(cameraPos, targetPoint, worldUp);
	//mat4 view = CustomLookAt(vec3(cameraPosition), vec3(0, 0, 0), vec3(0.0f, 1.0f, 0.0f));
	//mat4 view = translate(identityMat, vec3(0.0f, 0.0f, -5.0f));
	//mat4 view = translate(identityMat, vec3(1.0f, 0.0f, -5.0f));
	//view = rotate(view, radians(45.0f), vec3(0, 0, 1.0f));
	mat4 projMat = glm::perspective(radians(45.0f), (float)WindowApplication::WIDTH / WindowApplication::HEIGHT, 0.1f, 100.0f);
	//mat4 projMat = glm::perspective(radians(45.0f), 0.9f, 0.1f, 100.0f);

	//m_QShaderProgram->SetMatrix("view", view);
	//m_QShaderProgram->SetMatrix("projection", projMat);

	//mat4 modelMat = glm::translate(identityMat, vec3(0.5, 0.5, 0.0));

	////mat4 modelMat = glm::rotate(identityMat, 0.3f * static_cast<float>(glfwGetTime()), vec3(1.0f, 1.0f, 1.0f));
	////glm::mat4 rotate = glm::rotate(identityMat, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f) );
	///*if ( 0 != i % 3)
	//{*/
	////modelMat = rotate(modelMat, 0.3f * static_cast<float>(glfwGetTime()), vec3(1.0f, 1.0f, 1.0f));
	////}
	////mat4 projMat = identityMat;
	//glm::mat4  mvp = projMat * view * modelMat;
	//m_QShaderProgram->SetMatrix("mvp", mvp);

	////m_QShaderProgram->SetMatrix("model", modelMat);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	for (size_t i = 0; i < 10; i++)
	{
		// 更新MVP 矩阵
		const auto& elment = cubePositions[i];
		mat4 modelMat = glm::translate(identityMat, elment);
		//mat4 modelMat = glm::rotate(identityMat, 0.3f * static_cast<float>(glfwGetTime()), vec3(1.0f, 1.0f, 1.0f));
		//glm::mat4 rotate = glm::rotate(identityMat, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f) );
		/*if ( 0 != i % 3)
		{*/
		//modelMat = rotate(modelMat, 0.3f * static_cast<float>(glfwGetTime()), vec3(1.0f, 1.0f, 1.0f));
		//}
		//mat4 projMat = identityMat;
		glm::mat4  mvp = projMat * view * modelMat;
		m_QShaderProgram->SetMatrix("mvp", mvp);

		//m_QShaderProgram->SetMatrix("model", modelMat);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
}

void QHelloTexture::ShutDown()
{

	// optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void QHelloTexture::TestOutParam(QCamera& testCamera)
{

}

void QHelloTexture::InitAsset()
{

	//

	m_QShaderProgram->CreateAndSetup("4.1.texture.vs", "4.1.texture.fs");

	//SetupVertexData();
	SetupCubeVertexData();

	SetupTextures();

}



void QHelloTexture::InitGameplay()
{
	
	//QCamera::GetInstance().SetEyeAtUp(cameraPos, targetPoint, worldUp);
	QCamera::GetInstance().SetEyeAtUp(
		glm::vec3(0.0, 0.0, 3.0)
		, glm::vec3(0.0, 0.0, 0.0)
		, glm::vec3(0.0, 1.0, 0.0));
}

void QHelloTexture::SetupVertexData()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	//unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


void QHelloTexture::SetupTextures()
{
	// load and create a texture 
	// -------------------------
	/*unsigned int texture;*/
	glGenTextures(1, &texture);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
	// set the texture wrapping parameters
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
	unsigned char* data = stbi_load("Textures/wall.jpg", &width, &height, &nrChannels, 0);
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

#pragma region 上传第二张图片到GPU
	glGenTextures(1, &m_texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_texture2);

	// setup texture wrap mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	// setup filter mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image data
	int width2, height2, nrChannels2;
	unsigned char* data2 = stbi_load("Textures/awesomeface.jpg", &width2, &height2, &nrChannels2, 0);

	// upload data to GPU
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}
	stbi_image_free(data2);
#pragma endregion
}

void QHelloTexture::SetupCubeVertexData()
{
	// cube vertex data
	float vertices[] = {
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
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

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

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

glm::mat4 QHelloTexture::CustomLookAt(glm::vec3 cameraPosition, glm::vec3 targetPoint, glm::vec3 upVector)
{
	using namespace glm;
	mat4 identityMat = glm::identity<glm::mat4>();
	//mat4 translate = glm::translate(identityMat, (-1.0f) * cameraPosition);

	//vec3 forward = glm::normalize(cameraPosition - targetPoint);


	vec3 m_Forward = normalize(targetPoint - cameraPosition);
	vec3 m_Right = normalize(cross( m_Forward, upVector));
	vec3 m_Up = cross( m_Right, m_Forward);

	//glm::lookAt();
	vec3 xAsix = m_Right;
	vec3 yAxis = m_Up;
	vec3 zAxis = -m_Forward;
	vec3 eye = cameraPosition;


	mat4 Result = glm::mat4(1.0f);
	Result[0][0] = xAsix.x;
	Result[1][0] = xAsix.y;
	Result[2][0] = xAsix.z;
	Result[0][1] = yAxis.x;
	Result[1][1] = yAxis.y;
	Result[2][1] = yAxis.z;
	Result[0][2] = zAxis.x;
	Result[1][2] = zAxis.y;
	Result[2][2] = zAxis.z;
	Result[3][0] = -dot(xAsix, eye);
	Result[3][1] = -dot(yAxis, eye);
	Result[3][2] = -dot(zAxis, eye);
	//return Result;
	//m_ViewMatrix = Result;

	//vec<3, T, Q> const f(normalize(center - eye));
	//vec<3, T, Q> const s(normalize(cross(f, up)));
	//vec<3, T, Q> const u(cross(s, f));

	//mat<4, 4, T, Q> Result(1);
	//Result[0][0] = s.x;
	//Result[1][0] = s.y;
	//Result[2][0] = s.z;
	//Result[0][1] = u.x;
	//Result[1][1] = u.y;
	//Result[2][1] = u.z;
	//Result[0][2] = -f.x;
	//Result[1][2] = -f.y;
	//Result[2][2] = -f.z;
	//Result[3][0] = -dot(s, eye);
	//Result[3][1] = -dot(u, eye);
	//Result[3][2] = dot(f, eye);

	//vec<3, T, Q> const f(normalize(center - eye));
	//vec<3, T, Q> const s(normalize(cross(up, f)));
	//vec<3, T, Q> const u(cross(f, s));

	//mat<4, 4, T, Q> Result(1);
	//Result[0][0] = s.x;
	//Result[1][0] = s.y;
	//Result[2][0] = s.z;
	//Result[0][1] = u.x;
	//Result[1][1] = u.y;
	//Result[2][1] = u.z;
	//Result[0][2] = f.x;
	//Result[1][2] = f.y;
	//Result[2][2] = f.z;
	//Result[3][0] = -dot(s, eye);
	//Result[3][1] = -dot(u, eye);
	//Result[3][2] = -dot(f, eye);

	return Result;
}
