#pragma once
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

//class HrException : public std::runtime_error
//{
//public:
//	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
//	HRESULT Error() const { return m_hr; }
//private:
//	const HRESULT m_hr;
//};
#define  OUT 

inline void ThrowIfFailed(void* hr)
{
	if (nullptr == hr)
	{
		throw std::runtime_error("pointer is null");
	}
}



inline std::ostream& operator<< (std::ostream& outStream, glm::vec3 myVec)
{
	outStream << "(" << myVec.x <<  "," << myVec.y << "," << myVec.z << " )  ";

	return outStream;
}

using GPUResourceHandle = unsigned int;

namespace QHelper
{
	inline void CheckAndPrintGLError()
	{
		unsigned int glErrorCode = glGetError();
		if (glErrorCode)
		{
			std::string s = "GLError = " + glGetError();
			std::cout << s << std::endl;
			throw std::runtime_error(s);
		}
	}

	GPUResourceHandle LoadTextureAndUpload2GPU(const std::string& filePath);
	GPUResourceHandle LoadCubemapAndUpload2GPU(const std::vector<std::string>& cubemapFaces);

	/**
	 *  ��������к��ϸ���޶�Ҫ�������������������� �����ʽ��������Ŀ��3�ı�����
	 *	 	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};
	 */
	void GenerateStandardVABO(const std::vector<float>& vertices,  OUT GPUResourceHandle* pVAO,OUT GPUResourceHandle* pVBO);

	/** 
		arraySize �ϸ���˵���Ը�Ϊֱ�Ӵ��ڴ�ռ�ã������������confusing 
	*/
	void GenerateStandardVABO(const float* verticesArray, int arraySize, OUT GPUResourceHandle* pVAO, OUT GPUResourceHandle* pVBO);


	/**
	 *  ��������к��ϸ���޶�Ҫ�������������������� �����ʽ��������Ŀ��3�ı�����
	 *	 	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};
	 */
	void GenerateStandardVABO2(const float* verticesArray, int arraySize, OUT GPUResourceHandle* pVAO, OUT GPUResourceHandle* pVBO);
}