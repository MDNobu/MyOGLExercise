#pragma once
#include "QShader.h"
#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
#include "QMesh.h"

class QModel
{
public:
	void Draw(const QShader& shader) const;
	void LoadModel(const std::string& path);

private:
	void processNode(aiNode* node, const aiScene* scene);



	QMesh TransIntoQMesh(aiMesh* mesh, const aiScene* scene);

	GPUResourceHandle LoadTextureAndUpload2GPU(const std::string& filePath);


	//loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	void LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName,
		OUT std::vector<QTexture>& textureBuffer);

	// cached model directory, 方便处理texture时找路径
	std::string m_Directory;

	std::vector<QMesh> m_Meshes;

	std::unordered_map<std::string, QTexture> m_LoadedTextures;
};

