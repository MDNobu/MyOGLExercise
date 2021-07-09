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

QHelloTexture::QHelloTexture() : m_QShaderProgram(nullptr)
{
	m_QShaderProgram = std::unique_ptr<QShader>(new QShader());
}

void QHelloTexture::RenderScene()
{
	//throw std::logic_error("The method or operation is not implemented.");
	// render
	   // ------
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

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
	
	unsigned int uniformLocal = glGetUniformLocation(m_QShaderProgram->m_ShaderProgramID, "colorTint");
	glUniform4f(uniformLocal, 1.0f, 0.0f, 0.0f, 1.0f);

	// 更新MVP 矩阵
	glm::mat4 mvp = glm::mat4(1.0f);
	glm::mat4 translate = glm::translate(mvp, glm::vec3(0.5f, -0.5f, 0.0f));
	glm::mat4 rotate = glm::rotate(mvp, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f) );
	using namespace glm;
	mvp = translate * rotate   * mvp;
	unsigned int mvpLocation =glGetUniformLocation(m_QShaderProgram->m_ShaderProgramID, "mvp");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(mvp));

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void QHelloTexture::ShutDown()
{

	// optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void QHelloTexture::InitAsset()
{

	m_QShaderProgram->CreateAndSetup("4.1.texture.vs", "4.1.texture.fs");

	SetupVertexData();

	SetupTextures();

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