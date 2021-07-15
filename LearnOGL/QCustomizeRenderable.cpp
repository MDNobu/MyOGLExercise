#include "QCustomizeRenderable.h"





QCustomizeRenderable::QCustomizeRenderable(const float* vertexData, unsigned int arraySize, InitVertexDataType type)
{
	Init(vertexData, arraySize, type);
}



void QCustomizeRenderable::Draw(const QShader& shader)
{
	shader.Use();
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
	shader.SetMatrix("model", model);


	glBindVertexArray(m_VAO);
	if (m_HasIndices)
	{
		// #TODO DrawElements;
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_Vertices2.size());
	}
	glBindVertexArray(0);
}



void QCustomizeRenderable::Init(const float* vertexData, unsigned int arraySize, InitVertexDataType type)
{
	InitVertexData2(vertexData, arraySize, type);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	

	glBindVertexArray(m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QCustomizeVertex2) * m_Vertices2.size(), &m_Vertices2[0], GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QCustomizeVertex2), (void*)0);
	glEnableVertexAttribArray(0);

	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QCustomizeVertex2), (void*)(3 * sizeof(float)) );
	glEnableVertexAttribArray(1);

	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QCustomizeVertex2), (void*)(6* sizeof(float)) );
	glEnableVertexAttribArray(2);

	if (m_HasIndices)
	{
		glGenBuffers(1, &m_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);

#ifdef _DEBUG
	QHelper::CheckAndPrintGLError();
#endif // _DEBUG
}

void QCustomizeRenderable::InitVertexData1(const float* vertexData, unsigned int arraySize, InitVertexDataType type)
{
#pragma region initVertices
	using namespace std;
	using glm::vec3;
	QCustomizeVertex vertBuffer;
	switch (type)
	{
	case InitVertexDataType::POS_ONLY:
		assert(arraySize % 3 == 0);
		for (size_t i = 0; i < arraySize / 3; i++)
		{
			vertBuffer.m_Position = vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
			m_Vertices.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	case InitVertexDataType::POS_NORMAL:
		assert(arraySize % 6 == 0);
		for (size_t i = 0; i < arraySize / 6; i++)
		{
			vertBuffer.m_Position = vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
			vertBuffer.m_Normal = vec3(vertexData[i + 3], vertexData[i + 4], vertexData[i + 5]);
			m_Vertices.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	case InitVertexDataType::POS_UV:
		assert(arraySize % 5 == 0);
		for (size_t i = 0; i < arraySize / 5; i++)
		{
			vertBuffer.m_Position = vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
			vertBuffer.m_UV = glm::vec2(vertexData[i + 3], vertexData[i + 4]);
			m_Vertices.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	case InitVertexDataType::ALL:
		assert(arraySize % 8 == 0);
		for (size_t i = 0; i < arraySize / 8; i++)
		{
			vertBuffer.m_Position = vec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
			vertBuffer.m_Normal = vec3(vertexData[i + 3], vertexData[i + 4], vertexData[i + 5]);
			vertBuffer.m_UV = glm::vec2(vertexData[i + 6], vertexData[i + 7]);
			m_Vertices.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	default:
		break;
	}
#pragma endregion initVertices


}

void QCustomizeRenderable::InitVertexData2(const float* vertexData, unsigned int arraySize, InitVertexDataType type)
{
	using namespace std;
	QCustomizeVertex2 vertBuffer;
	//vertBuffer.Reset();
	switch (type)
	{
	case InitVertexDataType::POS_ONLY:
		assert(arraySize % 3 == 0);
		for (size_t i = 0; i < arraySize / 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				vertBuffer.m_Vertex[j] = vertexData[i * 3 + j];
			}
			m_Vertices2.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	case InitVertexDataType::POS_NORMAL:
		assert(arraySize % 6 == 0);
		for (size_t i = 0; i < arraySize / 6; i++)
		{
			for (size_t j = 0; j < 6; j++)
			{
				vertBuffer.m_Vertex[j] = vertexData[i * 6 + j];
			}
			m_Vertices2.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	case InitVertexDataType::POS_UV:
		assert(arraySize % 5 == 0);
		for (size_t i = 0; i < arraySize / 5; i++)
		{
			vertBuffer.m_Vertex[0] = vertexData[i * 5 + 0];
			vertBuffer.m_Vertex[1] = vertexData[i * 5 + 1];
			vertBuffer.m_Vertex[2] = vertexData[i * 5 + 2];
			vertBuffer.m_Vertex[6] = vertexData[i * 5 + 3];
			vertBuffer.m_Vertex[7] = vertexData[i * 5 + 4];
			m_Vertices2.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	case InitVertexDataType::ALL:
		assert(arraySize % 8 == 0);
		for (size_t i = 0; i < arraySize / 8; i++)
		{
			for (size_t j = 0; j < 8; j++)
			{
				vertBuffer.m_Vertex[j] = vertexData[i * 8 + j];
			}
			m_Vertices2.push_back(vertBuffer);
			vertBuffer.Reset();
		}
		break;
	default:
		break;
	}
}
