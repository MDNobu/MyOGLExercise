#include "QModel.h"


void QModel::Draw(const QShader& shader) const
{
	shader.Use();
	for (const QMesh& mesh : m_Meshes)
	{
		mesh.Draw(shader);
	}
}

void QModel::LoadModel(const std::string& path)
{
	// read file via ASSIMP
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
		|| !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		throw std::runtime_error("Failed to load model");
		return;
	}
	m_Directory = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);
}

void QModel::processNode(aiNode* node, const aiScene* scene)
{
	// 处理第一个节点
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		// node 里存储的是 在scene的mesh array里的索引，mesh实际是在scene中
		int meshIndexInScene = node->mMeshes[i];

		aiMesh* mesh = scene->mMeshes[meshIndexInScene];

		m_Meshes.push_back( TransIntoQMesh(mesh, scene));
	}


	// 递归子节点
	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

QMesh QModel::TransIntoQMesh(aiMesh* mesh, const aiScene* scene)
{
	using namespace std;
	vector<QVertex> vertices;
	vector<unsigned int> indices;
#pragma region 加载顶点信息
	// walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		QVertex vertex;
		glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
		// positions
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.m_Position = vector;
		// normals
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.m_Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.m_UV = vec;
			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.m_Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.m_Bitangent = vector;
		}
		else
			vertex.m_UV = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
#pragma endregion 加载顶点信息


	vector<QTexture> textures;
#pragma region 加载Texture信息
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN
	//material, aiTextureType_AMBIENT, "texture_height"
	LoadMaterialTextures(material, aiTextureType_DIFFUSE, QTexture::DIFFUSE, textures);
	LoadMaterialTextures(material, aiTextureType_SPECULAR, QTexture::SPECULAR, textures);
	LoadMaterialTextures(material, aiTextureType_HEIGHT, QTexture::NORMAL, textures);
	LoadMaterialTextures(material, aiTextureType_AMBIENT, QTexture::HEIGHT, textures);
#pragma endregion 加载Texture信息
	

	return QMesh(vertices, indices, textures);
}



//GPUResourceHandle QModel::LoadTextureAndUpload2GPU(const std::string& filePath)
//{
//
//	//string filename = string(path);
//	//filename = directory + '/' + filename;
//
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format = GL_RGB;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << filePath << std::endl;
//		stbi_image_free(data);
//	}
//	return textureID;
//}

void QModel::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, 
	OUT std::vector<QTexture>& textureBuffer)
{
	using namespace std;

	//string filePath = m_Directory + "/" + 
	for (size_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		string filePath = m_Directory + "/" + string(str.C_Str());

		auto iter = m_LoadedTextures.find(filePath);
		if (iter == m_LoadedTextures.end()) // 如果没有cache 则cache
		{
			QTexture texture;
			texture.m_ID = QHelper::LoadTextureAndUpload2GPU(filePath);
			texture.m_Type = typeName;
			texture.m_Filename = str.C_Str();
			
			m_LoadedTextures.insert(pair<string, QTexture>(filePath, texture));

			textureBuffer.push_back(texture);
		}
		else
		{
			textureBuffer.push_back(iter->second);
		}

		//textureBuffer.push_back(texture);
	}
}
