#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class QShader
{
public:
	QShader();
	~QShader();

	bool CreateAndSetup(const char* vsPath, const char* fsPath);
	void Use() ;
	void Cleanup() ;


	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}

	void SetMatrix(const std::string& name, const glm::mat4 & targetMat)
	{
		unsigned int matLocation = glGetUniformLocation(m_ShaderProgramID, name.c_str());
		glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(targetMat));
	}

	void setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), x, y, z);
	}
	
private:
	unsigned int m_ShaderProgramID;
	bool m_IsCleaned = false;

	void checkCompileErrors(unsigned int shader, std::string type);
};

