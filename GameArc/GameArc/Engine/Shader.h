#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
/** \class Shader
* \file Shader.h
* \brief A shader wrapper that loads and binds both shaders into one object
*
* This is the Open GL version, which finds, loads and binds the shaders into one object for easy use
* Orignal version: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*/
class Shader
{
public:
	///Shader ID (for GPU)
	unsigned int ID;
	///Constructor, finds and builds shaders for later use
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensure ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char * fShaderCode = fragmentCode.c_str();
		// 2. compile shaders
		unsigned int vertex, fragment;
		// vertex shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");
		// fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");
		// shader Program
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");
		// delete the shaders as they're linked into our program now and no longer necessary
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}
	///Activates shader
	void use()
	{
		glUseProgram(ID);
	}
	///Sends uniform boolean to GPU
	void setBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	///Sends uniform int to GPU
	void setInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	///Sends uniform float to GPU
	void setFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniform2f(glGetUniformLocation(this->ID, name), x, y);
	}
	void SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniform2f(glGetUniformLocation(this->ID, name), value.x, value.y);
	}
	void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniform3f(glGetUniformLocation(this->ID, name), x, y, z);
	}
	void SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniform3f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z);
	}
	void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniform4f(glGetUniformLocation(this->ID, name), x, y, z, w);
	}
	void SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniform4f(glGetUniformLocation(this->ID, name), value.x, value.y, value.z, value.w);
	}
	void SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader)
	{
		if (useShader)
			this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void SetArrayInt(const std::string &variableName, const std::string &elementName, int i, float value) const
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;

		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetArrayInt(const std::string &variableName, int i, float value) const
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "]";

		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	///Sends uniform float to GPU
	void SetArrayFloat(const std::string &variableName, const std::string &elementName, int i, float value) const
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;

		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void SetArrayVector2f(const std::string &variableName, const std::string &elementName, int i, GLfloat x, GLfloat y, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), x, y);
	}
	void SetArrayVector2f(const std::string &variableName, const std::string &elementName, int i, const glm::vec2 &value, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
	}
	void SetArrayVector3f(const std::string &variableName, const std::string &elementName, int i, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), x, y, z);
	}
	void SetArrayVector3f(const std::string &variableName, const std::string &elementName, int i, const glm::vec3 &value, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
	}
	void SetArrayVector4f(const std::string &variableName, const std::string &elementName, int i, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), x, y, z, w);
	}
	void SetArrayVector4f(const std::string &variableName, const std::string &elementName, int i, const glm::vec4 &value, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z, value.w);
	}
	void SetArrayMatrix4(const std::string &variableName, int i, const glm::mat4 &matrix, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "]";
		if (useShader)
			this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void SetArrayMatrix4(const std::string &variableName, const std::string &elementName, int i, const glm::mat4 &matrix, GLboolean useShader)
	{
		std::string name;

		name = variableName;
		name += "[";
		name += std::to_string(i);
		name += "].";
		name += elementName;
		if (useShader)
			this->use();
		glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

private:
	///Checks each shader to ensure it has been constructed correctly
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};