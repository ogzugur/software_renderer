#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class CShader
{
public:
	CShader(const GLchar* vertexPath, const GLchar* fragmentPath, std::string shaderName);
	CShader(const char *binaryPath, std::string shaderName);

	void use();
	void SetUniform(const std::string & pszName, const glm::vec3 mValue);
	void SetUniform(const std::string & pszName, const glm::vec2 mValue);
	void SetUniform(const std::string &pszName, const glm::mat4 mValue);
	void SetUniform(const char * pszName, const glm::vec3 mValue);
	void SetUniform(const char * pszName, const bool mValue);
	void SetUniform(const char * pszName, const int mValue);
	void SetUniform(const char * pszName, float mValue);
   std::string GetShaderName();
   size_t GetShaderID();

private:
   std::string shaderName;
   size_t ID;

   void checkCompileErrors(unsigned int shader, std::string type, std::string path);
};

#endif