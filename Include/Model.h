#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include <vector>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "stb_image.h"

class Model
{
public:

	std::vector<Mesh> meshs;
	std::vector<Texture> texture_loaded;
	Model(const char *path);
	void Draw(Shader shader);
	unsigned int TextureFromFile(const char* path, const std::string &directory, bool gammma);

private:
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};


#endif // !MODEL_H
