#pragma once
#include "QHelper.h"
#include <vector>
#include "QShader.h"

struct QVertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_UV;
};

struct QTexture
{
	unsigned int m_ID;
	std::string m_Type;
};

class QMesh
{
public:
	QMesh(const std::vector<QVertex>& vertices, const std::vector<unsigned int>& indices,const std::vector<QTexture>& textures);

	QMesh(const QMesh& other) = delete;
	~QMesh();
	//using std::vector;
	std::vector<QVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<QTexture> m_Textures;

	void Draw(const QShader& shader);

	void SetupGPUResources();
private:

	GPUResourceHandle m_VAO;
	GPUResourceHandle m_VBO;
	GPUResourceHandle m_EBO;

	bool m_IsGPUResourcesSetted  = false;
};

