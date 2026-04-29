#pragma once

#include <iostream>

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