#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Renderer.h"

class Shader
{
public:
	// Program ID
	unsigned int ID;

	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// Ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// Close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. compile shaders
		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		// vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		GLCall(glShaderSource(vertex, 1, &vShaderCode, NULL));
		GLCall(glCompileShader(vertex));

		GLCall(glGetShaderiv(vertex, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(vertex, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		GLCall(glShaderSource(fragment, 1, &fShaderCode, NULL));
		GLCall(glCompileShader(fragment));

		GLCall(glGetShaderiv(fragment, GL_COMPILE_STATUS, &success));
		if (!success) {
			GLCall(glGetShaderInfoLog(fragment, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		};

		// shader program
		ID = glCreateProgram();
		GLCall(glAttachShader(ID, vertex));
		GLCall(glAttachShader(ID, fragment));
		GLCall(glLinkProgram(ID));

		GLCall(glGetProgramiv(ID, GL_LINK_STATUS, &success));
		if (!success)
		{
			GLCall(glGetProgramInfoLog(ID, 512, NULL, infoLog));
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		};

		// Delete the shaders that are no longer necessary now that they're linked
		GLCall(glDeleteShader(vertex));
		GLCall(glDeleteShader(fragment));
	}
	// use/activate shader
	void use()
	{
		GLCall(glUseProgram(ID));
	}

	// Utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value));
	}
	void setInt(const std::string& name, int value) const
	{
		GLCall(glUniform1i(glGetUniformLocation(ID, name.c_str()), value));
	}
	void setFloat(const std::string& name, float value) const
	{
		GLCall(glUniform1f(glGetUniformLocation(ID, name.c_str()), value));
	}

	void setVec2(const std::string& name, const glm::vec2& value) const
	{
		GLCall(glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
	}
	void setVec2(const std::string& name, float x, float y) const
	{
		GLCall(glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y));
	}

	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		GLCall(glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
	}
	void setVec3(const std::string& name, float x, float y, float z) const
	{
		GLCall(glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z));
	}

	void setVec4(const std::string& name, const glm::vec4& value) const
	{
		GLCall(glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]));
	}
	void setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		GLCall(glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w));
	}

	void setMat2(const std::string& name, const glm::mat2& mat) const
	{
		GLCall(glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
	}

	void setMat3(const std::string& name, const glm::mat3& mat) const
	{
		GLCall(glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
	}

	void setMat4(const std::string& name, const glm::mat4& mat) const
	{
		GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
	}
};