#pragma once
#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include <assimp/postprocess.h>

struct  Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct  Texture
{
	unsigned int id;
	std::string type;
	aiString path;
};


class Mesh
{
public:
	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader shader);

private:
	unsigned int VBO, EBO;
	void setupMesh();

};


#endif // !MESH_H

