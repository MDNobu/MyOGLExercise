#pragma once

#include "IQRenderable.h"
#include "QHelper.h"
#include <vector>

// ֱ��д�ڴ�����Ķ������ݻ�����̫���ܳ�����������,tanget/bitanget�Ͳ���Ҫ��
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
	// 0 - 2 ���� 
	// 3-5 normal
	// 6/7 uv
	void Reset() {
		float tmp[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
		memcpy(m_Vertex, tmp, sizeof(tmp));
	}
};


//  #TODO Texture ��ػ�û���Ƶ�����
class QCustomizeRenderable : public IQRenderable
{
public:

	/*
	  ��������к��ϸ���޶�Ҫ�������������������� �����ʽ��4������ÿ������ֱ�float������3,6,5,8
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

	// ����ͳһ��QCustomizeVertex�� ��Ȼ����������ռ�ռ�ã��������ȼ�
	std::vector<QCustomizeVertex> m_Vertices;
	std::vector<QCustomizeVertex2> m_Vertices2;
	std::vector<unsigned int> m_Indices;

	GPUResourceHandle m_VAO = 0;
	GPUResourceHandle m_VBO = 0;
	GPUResourceHandle m_EBO = 0;
	bool m_HasIndices = false;
};

