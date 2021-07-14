#pragma once
#include "QHelper.h"



class QLight
{
public:
	enum class QLightType : char
	{
		DirectionalLight = 0,
		PointLight = 1,
		SpotLight = 2
	};

public:
	static QLight CreateTestLight();

public:
	QLightType m_LightType = QLightType::DirectionalLight;
	glm::vec3 m_Pos{0.0, 0.0, 0.0};   //Point light only
	glm::vec3 m_Color{1.0, 1.0, 1.0};
	glm::vec3 m_Direction{ 1.0, -1.0, 1.0 }; //DirectionalLight and SpotLight Only
	
	float m_Intensity = 1.0f;
#pragma region PointLight and Spotlight
	float m_FalloffStart = 10.0f;
	float m_FalloffEnd = 100.0f;
#pragma endregion PointLight and Spotlight

#pragma region SpotLight Only
	// SpotLight 开始衰减的角度和收束为0的角度
	//float m_InnerAngle = 30.0f;
	//float m_OuterAngle = 45.0f;
	// SpotLight 的 角度衰减参数pow(dot, SpotPower);
	float SpotPower = 1.0f;
#pragma endregion SpotLight Only
	 

};

