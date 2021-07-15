#include "QHelper.h"
#include "stb_image.h"

GPUResourceHandle QHelper::LoadTextureAndUpload2GPU(const std::string& filePath)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << filePath << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

GPUResourceHandle QHelper::LoadCubemapAndUpload2GPU(const std::vector<std::string>& cubemapFaces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < cubemapFaces.size(); i++)
	{
		unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << cubemapFaces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void QHelper::GenerateStandardVABO(const std::vector<float>& vertices, OUT GPUResourceHandle* pVAO, OUT GPUResourceHandle* pVBO)
{

	glGenVertexArrays(1, pVAO);
	glGenBuffers(1, pVBO);

	glBindVertexArray(*pVAO);
	glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void QHelper::GenerateStandardVABO2(const float* verticesArray, int arraySize, OUT GPUResourceHandle* pVAO, OUT GPUResourceHandle* pVBO)
{

}

void QHelper::GenerateStandardVABO(const float* verticesArray, int arraySize, OUT GPUResourceHandle* pVAO, OUT GPUResourceHandle* pVBO)
{
	glGenVertexArrays(1, pVAO);
	glGenBuffers(1, pVBO);

	glBindVertexArray(*pVAO);
	glBindBuffer(GL_ARRAY_BUFFER, *pVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * arraySize, verticesArray, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	
#ifdef _DEBUG
	CheckAndPrintGLError();
#endif // _DEBUG
}

