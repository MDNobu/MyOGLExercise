#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "QLight.h"
#include "QCamera.h"

class QShader
{
public:
	QShader();
	~QShader();

	bool CreateAndSetup(const char* vsPath, const char* fsPath);
	bool CreateAndSetup(const char* vsPath, const char* fsPath, const char* gsPath);
	

	void Use()  const;
	void Cleanup() ;


	void SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}

	void SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ShaderProgramID, name.c_str()), value);
	}


	void SetMatrix(const std::string& name, const glm::mat4 & targetMat) const
	{
		unsigned int matLocation = glGetUniformLocation(m_ShaderProgramID, name.c_str());
		glUniformMatrix4fv(matLocation, 1, GL_FALSE, glm::value_ptr(targetMat));
	}

	void SetVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), x, y, z);
	}
	void SetVec3(const std::string& name, glm::vec3 gvec) const
	{
		glUniform3f(glGetUniformLocation(m_ShaderProgramID, name.c_str()), 
			gvec.x, gvec.y, gvec.z);
	}

	void SetupLightUniforms(const QLight& light);

	void SetupCameraUniforms(const QCamera& camera)  const;
private:
	bool CreateAndBindGemetryShader(const char* gsPath);

	unsigned int m_ShaderProgramID;
	bool m_IsCleaned = false;

	void checkCompileErrors(unsigned int shader, std::string type);
};

