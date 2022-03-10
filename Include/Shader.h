#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>

class Shader
{
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometrypath = nullptr);
	void use();
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setMat4(const std::string &name, glm::mat4 mat);
	void setVec3(const std::string &name, float x, float y, float z);
	void setVec3(const std::string &name, glm::vec3 &value);
private:

};

#endif // !SHADER_H

