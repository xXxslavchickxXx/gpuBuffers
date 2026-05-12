#include "../Shader.h"

#include <iostream>
#include <sstream>
#include <fstream>

namespace shader {

	Shader::Shader(Shader&& other) noexcept {
		if (this != &other) {
			if (id) glDeleteShader(id);
			id = other.id;
			type = other.type;
			shaderSRC = std::move(other.shaderSRC);
			other.id = 0;
		}
	}
	Shader& Shader::operator=(Shader&& other) noexcept {
		if (this != &other) {
			if (id) glDeleteShader(id);
			id = other.id;
			type = other.type;
			shaderSRC = std::move(other.shaderSRC);
			other.id = 0;
		}
		return *this;
	}

	Shader::Shader(GLenum type, const std::string& source) : Shader(type, source.c_str()) {}
	Shader::Shader(GLenum type, const char* filepath) : type(type) {
		requireOpenGL();

		id = glCreateShader(type);

		std::ifstream file(filepath);
		if (!file) throw ShaderCompileError("Cannot open shader file: " + std::string(filepath));

		std::stringstream buffer;
		buffer << file.rdbuf();
		compile(buffer.str());
	}

	Shader::~Shader() {
		if (id) glDeleteShader(id);
	}

	Shader Shader::fromFile(GLenum type, const char* path) {
		return Shader(type, path);
	}
	Shader Shader::fromSource(GLenum type, const std::string& source) {
		return Shader(type, source);
	}

	void Shader::compile(const std::string& source) {
		shaderSRC = source;
		const char* src = shaderSRC.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetShaderInfoLog(id, 512, nullptr, infoLog);
			throw ShaderCompileError("Shader compilation failed\n" + std::string(infoLog));
		}
	}

}