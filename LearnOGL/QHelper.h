#pragma once
#include <stdexcept>

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
