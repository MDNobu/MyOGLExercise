#include "QLight.h"

QLight QLight::CreateTestLight()
{

	using glm::vec3;
#pragma region lightSettings
	const glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	const glm::vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	const vec3 lightDir = vec3(0.1f, -1.0f, -1.0f);
	const float shineness = 3.0f;
	const float ambientIntensity = 0.5f;
	const float intensity = 1.0f;
	const float falloffStart = 0.0f;
	const float falloffEnd = 10000.0f;
	const float spotPower = 1.0f;
#pragma endregion lightSettings
	QLight light;
	light.m_Intensity = intensity;
	light.m_Color = lightColor;

	light.m_Direction = lightDir;

	light.m_Pos = lightPos;
	light.m_FalloffStart = falloffStart;
	light.m_FalloffEnd = falloffEnd;

	light.SpotPower = spotPower;

	light.m_LightType = QLight::QLightType::DirectionalLight;
	return light;
}
