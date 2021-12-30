#include "CShader.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

CShader::CShader(const GLchar* vertexPath, const GLchar* fragmentPath, std::string shaderName)
{
	this->shaderName = shaderName;
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << shaderName << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX", vertexPath);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
   checkCompileErrors(fragment, "FRAGMENT", fragmentPath);


	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
   checkCompileErrors(ID, "PROGRAM", shaderName);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

CShader::CShader(const char *binaryPath, std::string shaderName)
{
	GLint binaryLength;
	void* binary;
	GLint success;
	FILE* infile;

	this->shaderName = shaderName;
	infile = fopen(binaryPath, "rb");
	fseek(infile, 0, SEEK_END);
	binaryLength = (GLint)ftell(infile);
	binary = (void*)malloc(binaryLength);
	fseek(infile, 0, SEEK_SET);
	fread(binary, binaryLength, 1, infile);
	fclose(infile);

	GLint formats = 0;
	glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
	GLint *binaryFormats = (GLint*)malloc(formats);
	glGetIntegerv(GL_PROGRAM_BINARY_FORMATS, binaryFormats);

    ID = glCreateProgram();
	glProgramBinary(ID, *binaryFormats, binary, binaryLength);

}

void CShader::use()
{
	glUseProgram(ID);
}
void CShader::SetUniform(const std::string &pszName, const glm::vec3 mValue)
{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName.c_str());
	glUniform3fv(iUniform, 1, glm::value_ptr(mValue));
}
void CShader::SetUniform(const std::string &pszName, const glm::vec2 mValue)
{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName.c_str());
	glUniform2fv(iUniform, 1, glm::value_ptr(mValue));
}
void CShader::SetUniform(const char* pszName, const glm::vec3 mValue)
{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName);
	glUniform3fv(iUniform, 1, glm::value_ptr(mValue));
}

void CShader::SetUniform(const std::string &pszName, const glm::mat4 mValue)

{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName.c_str());
	glUniformMatrix4fv(iUniform, 1, false, glm::value_ptr(mValue));
}
void CShader::SetUniform(const char* pszName, const bool mValue)

{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName);
	glUniform1i(iUniform, mValue);
}

void CShader::SetUniform(const char* pszName, const int mValue)

{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName);
	glUniform1i(iUniform, mValue);
}
void CShader::SetUniform(const char* pszName, float mValue)

{
	int iUniform = glGetUniformLocation((GLuint)ID, pszName);
	glUniform1f(iUniform, mValue);
}
void CShader::checkCompileErrors(unsigned int shader, std::string type, std::string path)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
         std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << " -> " << path << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
      else
      {
         std::cout << "SHADER COMPILED SUCCESFULLY -> " << path << std::endl;
      }
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
         std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << " -> " << path << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
      else
      {
         std::cout << "SHADER LINKED SUCCESFULLY -> " << path << "\n\n";
      }
	}
}

std::string CShader::GetShaderName()
{
   return shaderName;
}

size_t CShader::GetShaderID()
{
   return ID;
}