#pragma once
#include <string>
#include <glad/glad.h>

class QShader
{
public:
	QShader();

	bool CreateAndSetup(const char* vsPath, const char* fsPath);
	void Use() ;
	void Cleanup() ;
	void CreateAndSetup1();


	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}
	unsigned int m_ShaderProgramID;
private:
	

	void checkCompileErrors(unsigned int shader, std::string type);
};

