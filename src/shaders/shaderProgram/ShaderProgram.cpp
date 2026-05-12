#include "../ShaderProgram.h"

#include <iostream>
#include <format>

namespace shader {
	GLint ShaderProgram::getLocation(std::string name) {
		auto iter = nameToLocation.find(name);
		if (iter == nameToLocation.end()) return -1;
		return nameToLocation[name];
	}

	void ShaderProgram::reflectAttributes() {
		GLint numAttributes = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTES, &numAttributes);
		if (numAttributes == 0) return;

		GLint maxNameLength = 0;
		glGetProgramiv(programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxNameLength);
		std::vector<GLchar> nameBuffer(maxNameLength);

		for (GLint i = 0; i < numAttributes; i++) {
			GLsizei length = 0;
			GLint size = 0;
			GLenum type = 0;

			glGetActiveAttrib(programId, i, nameBuffer.size(), &length, &size, &type, nameBuffer.data());

			std::string name(nameBuffer.data(), length);
			GLint location = glGetAttribLocation(programId, name.c_str());

			nameToLocation[name] = location;
		}
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	:	programId(other.programId)
	{
		other.programId = 0;
	}
	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept {
		if (this != &other) {
			if (programId) glDeleteProgram(programId);
			programId = other.programId;
			other.programId = 0;
		}
		return *this;
	}

	ShaderProgram::ShaderProgram(Shader&& vertex, Shader&& fragment) {
		programId = glCreateProgram();
		attach(std::move(vertex));
		attach(std::move(fragment));
		link();

		bind();

		reflectAttributes();
	}
	ShaderProgram::ShaderProgram(const char* vertPath, const char* fragPath)
	:	ShaderProgram(std::move(Shader::fromFile(GL_VERTEX_SHADER, vertPath)),
		std::move(Shader::fromFile(GL_FRAGMENT_SHADER, fragPath))) {
	}

	ShaderProgram::~ShaderProgram() {
		if (programId) glDeleteProgram(programId);
	}

	void ShaderProgram::attach(Shader&& shader) {
		glAttachShader(programId, shader.getId());
	}
	void ShaderProgram::link() {
		glLinkProgram(programId);
		checkLinkStatus();
	}

	void ShaderProgram::bind() const {
		if (programId == 0) {
			throw std::runtime_error("This program of shaders is moved");
		}

		glUseProgram(programId);
	}

	void ShaderProgram::checkLinkStatus() {
		GLint success;
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infoLog[512];
			glGetProgramInfoLog(programId, 512, nullptr, infoLog);
			std::cerr << infoLog << std::endl;
			throw std::runtime_error(std::format("Shader program linking failed:\n{}", std::string(infoLog)));
		}
	}
}