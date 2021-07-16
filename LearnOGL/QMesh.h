#pragma once
#include "QHelper.h"
#include <vector>
#include "QShader.h"

struct QVertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_UV;	
	glm::vec3 m_Tangent;
	glm::vec3 m_Bitangent;
};
	
struct QTexture
{
	GPUResourceHandle m_ID;
	std::string m_Filename;

	using TextureType = std::string;
	TextureType m_Type;

	/// 注意这里涉及到 shader 内texture命名规则，必须是以下常量 + index，index和texture 语义后的一致
	const static TextureType DIFFUSE;
	const static TextureType SPECULAR;
	const static TextureType NORMAL;
	const static TextureType HEIGHT;

};



class QMesh
{
public:
	QMesh(const std::vector<QVertex>& vertices,
		const std::vector<unsigned int>& indices,const std::vector<QTexture>& textures);

	//QMesh(const QMesh& other) = delete;
	~QMesh();
	//using std::vector;
	std::vector<QVertex> m_Vertices;
	std::vector<unsigned int> m_Indices;
	std::vector<QTexture> m_Textures;

	void Draw(const QShader& shader) const;

	void SetupGPUResources();
private:

	GPUResourceHandle m_VAO = 0;
	GPUResourceHandle m_VBO  = 0;
	GPUResourceHandle m_EBO = 0;

	bool m_IsGPUResourcesSetted  = false;
};

