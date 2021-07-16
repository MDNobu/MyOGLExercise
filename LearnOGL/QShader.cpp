#include "QShader.h"

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "QHelper.h"

extern const char* g_VertexShaderSource;
extern const char* g_FragmentShaderSource;

QShader::QShader() : m_ShaderProgramID(0)
{

}

QShader::~QShader()
{
	if (!m_IsCleaned)
	{
		Cleanup();
	}
}

bool QShader::CreateAndSetup(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	m_ShaderProgramID = glCreateProgram();
	glAttachShader(m_ShaderProgramID, vertex);
	glAttachShader(m_ShaderProgramID, fragment);
	glLinkProgram(m_ShaderProgramID);
	checkCompileErrors(m_ShaderProgramID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

bool QShader::CreateAndSetup(const char* vsPath, const char* fsPath, const char* gsPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vsPath);
		fShaderFile.open(fsPath);
		gShaderFile.open(gsPath);
		std::stringstream vShaderStream, fShaderStream, gShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		gShaderStream << gShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		gShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		geometryCode = gShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment,geometry;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	checkCompileErrors(geometry, "GEOMETRY");

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	m_ShaderProgramID = glCreateProgram();
	glAttachShader(m_ShaderProgramID, vertex);
	glAttachShader(m_ShaderProgramID, fragment);
	glAttachShader(m_ShaderProgramID, geometry);
	glLinkProgram(m_ShaderProgramID);
	checkCompileErrors(m_ShaderProgramID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	//bool gsSuccess = CreateAndBindGemetryShader(gsPath);
	glDeleteShader(geometry);
	//return res && gsSuccess;
	return true;
}



// #TODO 代码中有大量的重复的同一个shader对象的use 可能造成pipeline的overhead ，这里要不要优化??
void QShader::Use() const
{
	//throw std::logic_error("The method or operation is not implemented.");
	glUseProgram(m_ShaderProgramID);
}


void QShader::Cleanup() 
{
	glDeleteProgram(m_ShaderProgramID);
	m_IsCleaned = true;
}


void QShader::SetupLightUniforms(const QLight& light)
{

	// TODO  这里 gpu的light对象是一个写死不能扩展的，后续考虑更新
	SetInt("lightObj.LightType", static_cast<int>(light.m_LightType));
	SetFloat("lightObj.Intensity", light.m_Intensity);
	SetVec3("lightObj.Color", light.m_Color);

	switch (light.m_LightType)
	{
	case QLight::QLightType::DirectionalLight:
		SetVec3("lightObj.DirectionWS", light.m_Direction);
		break;
	case QLight::QLightType::PointLight:
		SetVec3("lightObj.PositionWS", light.m_Pos);
		SetFloat("lightObj.FalloffStart", light.m_FalloffStart);
		SetFloat("lightObj.FalloffEnd", light.m_FalloffEnd);
		break;
	case QLight::QLightType::SpotLight:
		SetVec3("lightObj.DirectionWS", light.m_Direction);
		SetFloat("lightObj.FalloffStart", light.m_FalloffStart);
		SetFloat("lightObj.FalloffEnd", light.m_FalloffEnd);
		SetFloat("lightObj.SpotPower", light.SpotPower);
		break;
	default:
		assert(false);
		break;
	}
	QHelper::CheckAndPrintGLError();
}

void QShader::SetupCameraUniforms(const QCamera& camera) const
{
	using namespace glm;
	mat4 view = camera.GetViewMatrix();
	mat4 proj = camera.GetProjMatrix();
	//mat4 mvp = proj * view * model;
	//myShader.SetMatrix("model", model);
	//SetMatrix("mvp", mvp);
	SetMatrix("view", camera.GetViewMatrix());
	SetMatrix("projection", camera.GetProjMatrix());
	mat4 viewProj = camera.GetProjMatrix() * camera.GetViewMatrix();
	SetMatrix("viewProjection", viewProj);
}

bool QShader::CreateAndBindGemetryShader(const char* gsPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string gsCode;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		gShaderFile.open(gsPath);
		std::stringstream gShaderStream;
		// read file's buffer contents into streams
		gShaderStream << gShaderFile.rdbuf();
		// close file handlers
		gShaderFile.close();
		// convert stream into string
		gsCode = gShaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* gShaderCode = gsCode.c_str();
	// 2. compile shaders
	unsigned int geometry;
	// vertex shader
	geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	checkCompileErrors(geometry, "GEOMETRY");

	// shader Program
	//m_ShaderProgramID = glCreateProgram();
	glAttachShader(m_ShaderProgramID, geometry);
	glLinkProgram(m_ShaderProgramID);
	checkCompileErrors(m_ShaderProgramID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(geometry);

	return true;
}

// utility function for checking shader compilation/linking errors.
	// ------------------------------------------------------------------------
void QShader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			throw std::runtime_error("shader compile error");
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			throw std::runtime_error("shader compile error");
		}
	}
}