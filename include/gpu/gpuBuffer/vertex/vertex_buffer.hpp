#pragma once

#include <buffer.hpp>

namespace ag {
	class vertex_buffer : public buffer {
	public:
		using buffer::buffer;

	public:
		vertex_buffer() : buffer(GL_ARRAY_BUFFER) {}
		vertex_buffer(GLenum usage) : buffer(GL_ARRAY_BUFFER, usage) {}

		template<typename T>
		void upload(const T* data, size_t count) {
			allocate(sizeof(T) * count, data);
		}
		template<typename Container>
		void upload(const Container& container) {
			upload(container.data(), container.size());
		}

		template<typename T>
		void upload_part(const T* data, size_t count, size_t offset = 0) {
			set_sub_data(sizeof(T) * count, data, offset);
		}
		template<typename Container>
		void upload_part(const Container& container, size_t offset = 0) {
			set_sub_data(sizeof(container), container.data(), offset);
		}
	};
}