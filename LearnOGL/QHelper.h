#pragma once
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
}