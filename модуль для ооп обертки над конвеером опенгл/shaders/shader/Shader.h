#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>

// Под базовым классом шейдер, есть функция по проверке наличия контекста

namespace shader {
	enum ShaderType { VERTEX = GL_VERTEX_SHADER, FRAGMENT = GL_FRAGMENT_SHADER };

	class ShaderCompileError : public std::exception {
		std::string message;
	public:
		ShaderCompileError(const std::string& msg) : message(msg) {}
		const char* what() const noexcept override { return message.c_str(); }
	};

	class Shader {
		GLuint id = 0;
		GLenum type;
		std::string shaderSRC;

	public:
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;

		Shader(Shader&& other) noexcept;
		Shader& operator=(Shader&& other) noexcept;

		Shader(GLenum type, const std::string& source);
		Shader(GLenum type, const char* filepath);

		~Shader();

		GLuint getId() const { return id; }
		GLenum getType() const { return type; }

		static Shader fromFile(GLenum type, const char* path);
		static Shader fromSource(GLenum type, const std::string& source);

	private:
		void compile(const std::string& source);
	};

	inline void requireOpenGL() {
		static bool checked = false;

		if (!checked) {
			const GLubyte* glewVersion = glewGetString(GLEW_VERSION);
			if (glewVersion == nullptr) {
				throw std::runtime_error(
					"GLEW не инициализирован!\n"
					"Убедитесь, что вы:\n"
					"1. Создали OpenGL контекст (GLFW, SDL, и т.д.)\n"
					"2. Вызвали glewInit() после создания контекста"
				);
			}

			GLint majorVersion = 0;
			glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);

			GLenum err = glGetError();
			if (err != GL_NO_ERROR || majorVersion == 0) {
				throw std::runtime_error(
					"OpenGL контекст не активен или поврежден"
				);
			}

			checked = true;
		}
	}
}