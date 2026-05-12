#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

/// FWD тайп ту гл трейтов
template<typename T>
struct type_to_gltype;

template<> struct type_to_gltype<glm::vec2> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 2;
};
template<> struct type_to_gltype<glm::vec3> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 3;
};
template<> struct type_to_gltype<glm::vec4> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 4;
};

template<> struct type_to_gltype<glm::mat2> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 4;
};
template<> struct type_to_gltype<glm::mat3> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 9;
};
template<> struct type_to_gltype<glm::mat4> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 16;
};