#pragma once

#include <unordered_map>
#include <shader/Shader.h>

namespace shader {
	class ShaderProgram {
		GLuint programId = 0;

		std::unordered_map<std::string, GLint> nameToLocation;

	public:
		void attach(Shader&& shader);
		void link();

		void bind() const;
		static void unbind() { glUseProgram(0); }

		void deleteProgram() { glDeleteProgram(programId); }
		GLuint getId() { return programId; }

	public:
		GLint getLocation(std::string name);
		size_t getAttributesSize() { return nameToLocation.size(); }

	public:
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		ShaderProgram(ShaderProgram&& other) noexcept;
		ShaderProgram& operator=(ShaderProgram&& other) noexcept;

		ShaderProgram() = default;
		ShaderProgram(Shader&& vertex, Shader&& fragment);
		ShaderProgram(const char* vertPath, const char* fragPath);

		~ShaderProgram();

		explicit operator bool() const { return programId != 0; }

	private:
		void reflectAttributes();
		void checkLinkStatus();
	};
}