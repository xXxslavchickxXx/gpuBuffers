#pragma once

// FWD структуры для примера
struct vec3;

/// FWD тайп ту гл трейтов
template<typename T>
struct type_to_gltype;

template<> struct type_to_gltype<vec3> {
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 3;
};