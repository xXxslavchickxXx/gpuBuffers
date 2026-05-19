#pragma once

#include <buffer.hpp>

namespace ag {
	class indexed_buffer : public buffer {
	public:
		using buffer::buffer;

	public:
		indexed_buffer() : buffer(GL_ELEMENT_ARRAY_BUFFER) {}
		indexed_buffer(GLenum usage) : buffer(GL_ELEMENT_ARRAY_BUFFER, usage) {}

		template<typename T>
		void upload(const T* data, size_t count) {
			static_assert(std::is_same_v<T, uint32_t> || std::is_same_v<T, uint16_t>,
				"Indices must be 16 or 32 bit");
			allocate(sizeof(T) * count, data);
		}
		template<typename Container>
		void upload(const Container& container) {
			upload(container.data(), container.size());
		}

		template<typename T>
		void upload_part(const T* data, size_t count, size_t offset = 0) {
			static_assert(std::is_same_v<T, uint32_t> || std::is_same_v<T, uint16_t>,
				"Indices must be 16 or 32 bit");
			set_sub_data(sizeof(T) * count, data, offset);
		}
		template<typename Container>
		void upload_part(const Container& container, size_t offset = 0) {
			set_sub_data(container.size(), container.data(), offset);
		}
	};
}