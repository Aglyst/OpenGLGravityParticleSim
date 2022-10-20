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

private:
	std::string Parse(std::string path);
	void CheckErrors(GLuint shaderID, const char* type);
};

