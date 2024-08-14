#include "Shader.h"

Shader::Shader(std::string vertPath, std::string fragPath)
{
	GLuint vertID = glCreateShader(GL_VERTEX_SHADER);
	std::string vertShaderString = Parse(vertPath);
	const char* vertShaderSource = vertShaderString.c_str();
	glShaderSource(vertID, 1, &vertShaderSource, NULL);
	glCompileShader(vertID);
	CheckErrors(vertID, "VERT");

	GLuint fragID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragShaderString = Parse(fragPath);
	const char* fragShaderSource = fragShaderString.c_str();
	glShaderSource(fragID, 1, &fragShaderSource, NULL);
	glCompileShader(fragID);
	CheckErrors(fragID, "FRAG");

	progID = glCreateProgram();
	glAttachShader(progID, vertID);
	glAttachShader(progID, fragID);
	glLinkProgram(progID);
	CheckErrors(progID, "PROGRAM");

	glDetachShader(progID, vertID);;
	glDeleteShader(vertID);
	glDetachShader(progID, fragID);
	glDeleteShader(fragID);
}

void Shader::Activate()
{
	glUseProgram(progID);
}

void Shader::Delete()
{
	glDeleteProgram(progID);
}

void Shader::SetFloat(std::string name, float value) 
{
	glUniform1f(glGetUniformLocation(progID, name.c_str()), value);
}

std::string Shader::Parse(std::string path)
{
	std::ifstream in(path, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents.c_str());
	}
	throw(errno);
}

void Shader::CheckErrors(GLuint shaderID, const char* type)
{
	GLint hasCompiled;

	char infoLog[1024];

	if (type != "PROGRAM")
	{
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shaderID, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}
