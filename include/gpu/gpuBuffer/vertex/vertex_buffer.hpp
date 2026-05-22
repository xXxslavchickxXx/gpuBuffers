#pragma once

#include <buffer.hpp>

namespace ag {
	namespace gpu {
		class vertex_array;
	}

	class vertex_buffer : private buffer {
		friend ag::gpu::vertex_array;

		GLint compCount = -1;
		GLint byteSize = -1;
		GLenum type = -1;
		GLboolean normalized = -1;

		bool is_init = false;

		void set_format(GLint compCount, GLint byteSize, GLenum type, GLboolean normalized) {
			this->compCount = compCount; this->byteSize = byteSize; this->type = type; this->normalized = normalized; is_init = true;
		}

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
			set_sub_data(container.size() * sizeof(typename Container::value_type), container.data(), offset);
		}
	};
}