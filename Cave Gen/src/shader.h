#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int progID;

	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
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
		catch (std::ifstream::failure e) {
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
		}
		const char *vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::VERTEX::COULD_NOT_COMPILE\n" <<  infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::FRAGMENT::COULD_NOT_COMPILE\n" <<  infoLog << std::endl;
		}

		progID = glCreateProgram();
		glAttachShader(progID, vertex);
		glAttachShader(progID, fragment);
		glLinkProgram(progID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	};

	void use() {
		glUseProgram(progID);
	};
	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(progID, name.c_str()), (int)value);
	};
	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(progID, name.c_str()), value);
	};
	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(progID, name.c_str()), value);
	};
	void setMat4(const std::string& name, glm::mat4 mat) const {
		glUniformMatrix4fv(glGetUniformLocation(progID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
};

#endif