#pragma once

#include <iostream>
#include <GL/glew.h>

#include "userTraits.h"
#include "glmTraits.h"

// Трейты для ОГЛ
template<typename T>
struct type_to_gltype {};

template<> struct type_to_gltype<float> { 
	static const GLenum type = GL_FLOAT;
	static const GLenum components = 1;
};
template<> struct type_to_gltype<int> {
	static const GLenum type = GL_INT;
	static const GLenum components = 1;
};
template<> struct type_to_gltype<unsigned int> {
	static const GLenum type = GL_UNSIGNED_INT;
	static const GLenum components = 1;
};
template<> struct type_to_gltype<bool> {
	static const GLenum type = GL_BOOL;
	static const GLenum components = 1;
};

template<typename T>
constexpr GLenum type_to_gltype_t = type_to_gltype<T>::type;

template<typename T>
constexpr GLenum type_to_gltype_c = type_to_gltype<T>::components;

// Проверка на наличие метода size()
template<typename T, typename = void>
struct have_size : std::false_type {};

template<typename T>
struct have_size<T,
	std::void_t<decltype(std::declval<T>().size())>
> : std::true_type {};

template<typename T>
constexpr bool have_size_v = have_size<T>::value;

// Проверка на наличие метода data()
template<typename T, typename = void>
struct have_data : std::false_type {};

template<typename T>
struct have_data<T,
	std::void_t<decltype(std::declval<T>().data())>
> : std::true_type {};

template<typename T>
constexpr bool have_data_v = have_data<T>::value;