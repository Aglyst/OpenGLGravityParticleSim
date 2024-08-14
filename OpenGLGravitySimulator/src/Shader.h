#pragma once

#include<iostream>
#include<fstream>
#include<sstream>
#include<cerrno>

#include <glad/glad.h>

class Shader
{
public:
	GLuint progID;

	Shader(std::string vertPath, std::string fragPath);
	void Activate();
	void Delete();

	void SetFloat(std::string name, float value);

private:
	std::string Parse(std::string path);
	void CheckErrors(GLuint shaderID, const char* type);
};

