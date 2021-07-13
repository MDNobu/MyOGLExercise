#include "QMesh.h"
#include "QCamera.h"

QMesh::QMesh(const std::vector<QVertex>& vertices, 
	const std::vector<unsigned int>& indices, const std::vector<QTexture>& textures) : 
	m_Vertices(vertices), m_Indices(indices), m_Textures(textures)
{
	
}

QMesh::~QMesh()
{

}

void QMesh::Draw(const QShader& myShader)
{
	assert(m_IsGPUResourcesSetted, "you must setup gpu resource first");

	glBindVertexArray(m_VAO);
	myShader.Use();

#pragma region updateShaderUniforms
	using namespace glm;
	mat4 identityMat = mat4(1.0f);
	mat4 model = identityMat;
	mat4 view = QCamera::GetInstance().GetViewMatrix();
	mat4 proj = QCamera::GetInstance().GetProjMatrix();
	mat4 mvp = proj * view * model;
	myShader.SetMatrix("model", model);
	myShader.SetMatrix("mvp", mvp);

	// TODO 光照设置常量先直接放在这里，后续添加light对象之后移过去
#pragma region lightSettings
	const glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);
	const glm::vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	const float shineness = 3.0f;
	const float ambientIntensity = 0.5f;
#pragma endregion lightSettings
	myShader.SetVec3("objectColor", objectColor);
	myShader.SetVec3("lightColor", lightColor);
	// 这里先假设 lightDir是这样
	//myShader.SetVec3("uLightPosWS", m_LightPos);

	myShader.SetFloat("ambientIntensity", ambientIntensity);

	myShader.SetFloat("cubeMaterial.shineness", shineness);

	//m_NonLightShader->SetS
	myShader.SetInt("cubeMaterial.baseColorMap", 0);
	myShader.SetInt("cubeMaterial.specularMap", 1);
	myShader.SetInt("cubeMaterial.emissiveMap", 2);
#pragma endregion



	glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

}

void QMesh::SetupGPUResources()
{

#pragma region SetupBuffers
	// QVertex  必须是plain old object才能直接提交到pipeline
	static_assert(std::is_pod<QVertex>::value, "QVertex must be pod");

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(QVertex), &(m_Vertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), &m_Indices[0], GL_STATIC_DRAW);


	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(QVertex), (void*)0);
	glEnableVertexAttribArray(0);

	// normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(QVertex), (void*)offsetof(QVertex, m_Normal));
	glEnableVertexAttribArray(1);

	// uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(QVertex), (void*)offsetof(QVertex, m_UV));
	glEnableVertexAttribArray(2);


	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//g	lBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	QHelper::CheckAndPrintGLError();
#pragma endregion SetupBuffers


#pragma region SetupTextures

#pragma endregion SetupTextures

	m_IsGPUResourcesSetted = true;
}

