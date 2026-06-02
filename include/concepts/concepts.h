#pragma once

#include <iostream>

namespace ag::concepts {
	template<typename T>
	concept Container = requires (T value) {
		typename T::value_type;
		value.size();
		value.begin();
		value.end();
	};

	template<typename T>
	concept Scalar = std::is_scalar_v<T>;

	template<typename T>
	concept TriviallyCopyable = std::is_trivially_copyable_v<T>;
}