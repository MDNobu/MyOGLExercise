#pragma once

#include "IQRenderable.h"
#include "QHelper.h"
#include <vector>

// 直接写在代码里的顶点数据基本不太可能超过下面三种,tanget/bitanget就不需要了
struct QCustomizeVertex
{
public:
	void Reset() {
		m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_Normal = glm::vec3(0.0f, 0.0f, 1.0f);
		m_UV = glm::vec2(0.0f, 0.0f);
	}

	glm::vec3 m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_Normal = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec2 m_UV = glm::vec2(0.0f, 0.0f);
};
struct QCustomizeVertex2
{
public:
	float m_Vertex[8] = { 0, 0, 0, 0, 0, 1, 0, 0 };
	// 0 - 2 顶点 
	// 3-5 normal
	// 6/7 uv
	void Reset() {
		float tmp[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		memcpy(m_Vertex, tmp, sizeof(tmp));
	}
};


//  #TODO Texture 相关还没有移到这里
class QCustomizeRenderable : public IQRenderable
{
public:

	/*
	  这个方法有很严格的限定要求输入是像下面这样的 顶点格式，4中类型每个顶点分别float数量是3,6,5,8
	 *	 	float transparentVertices[] = {
		// positions         // normal,       // uv
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,   0.0f, 0.0f,

	*/
	enum class InitVertexDataType
	{
		POS_ONLY,
		POS_NORMAL,
		POS_UV,
		ALL
	};

public:
	QCustomizeRenderable(const float* vertexData, unsigned int arraySize, InitVertexDataType type);
public:
	virtual void Draw(const QShader& shader) override;


private:
	void Init(const float* vertexData, unsigned int arraySize, InitVertexDataType type);

	void InitVertexData1(const float* vertexData, unsigned int arraySize, InitVertexDataType type);
	void InitVertexData2(const float* vertexData, unsigned int arraySize, InitVertexDataType type);

	// 这里统一存QCustomizeVertex， 虽然可能有冗余空间占用，但现在先简化
	std::vector<QCustomizeVertex> m_Vertices;
	std::vector<QCustomizeVertex2> m_Vertices2;
	std::vector<unsigned int> m_Indices;

	GPUResourceHandle m_VAO = 0;
	GPUResourceHandle m_VBO = 0;
	GPUResourceHandle m_EBO = 0;
	bool m_HasIndices = false;
};

