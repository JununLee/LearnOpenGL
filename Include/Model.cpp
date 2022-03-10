#include "Model.h"

Model::Model(const char * path)
{
	loadModel(path);
}

void Model::Draw(Shader shader)
{
	for (unsigned int i = 0; i < meshs.size(); i++)
	{
		meshs[i].Draw(shader);
	}
}

unsigned int Model::TextureFromFile(const char * path, const std::string & directory, bool gammma)
{
	std::string fileName = std::string(path);
	fileName = directory + '/' + fileName;
	unsigned int textureId;
	glGenTextures(1, &textureId);
	int width, heigth, nrChannel;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &heigth, &nrChannel, 0);
	if (data) {
		GLenum format;
		if (nrChannel == 1)
			format = GL_RED;
		else if (nrChannel == 3)
			format = GL_RGB;
		else if (nrChannel == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, heigth, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed load:: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureId;
}

void Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags&AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshs.push_back(processMesh(mesh, scene));
	}
	for ( unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;
		if (mesh->mTextureCoords[0]) {
			glm::vec2 texcoord;
			texcoord.x = mesh->mTextureCoords[0][i].x;
			texcoord.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = texcoord;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0);
		}

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		for ( unsigned int j = 0; j < texture_loaded.size(); j++)
		{
			if (std::strcmp(str.C_Str(), texture_loaded[i].path.C_Str()) == 0) {
				textures.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), directory, false);
			texture.type = typeName;
			texture.path = str.C_Str();
			texture_loaded.push_back(texture);
			textures.push_back(texture);
		}
	}
	return textures;
}
