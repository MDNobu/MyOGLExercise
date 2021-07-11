#pragma once
#include <stdexcept>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//class HrException : public std::runtime_error
//{
//public:
//	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
//	HRESULT Error() const { return m_hr; }
//private:
//	const HRESULT m_hr;
//};

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