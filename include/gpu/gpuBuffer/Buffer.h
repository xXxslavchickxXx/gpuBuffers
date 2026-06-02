#pragma once

#include <GL/glew.h>
#include <concepts.h>
#include <traits.h>

namespace ag {
	class buffer {
	protected:
		GLuint id;
		GLenum target;
		GLenum usage;

		size_t size = 0;
		size_t capacity = 0;

	public:
		buffer(GLenum target, GLenum usage = GL_DYNAMIC_DRAW) : target(target), usage(usage), size(0)
		{
			glGenBuffers(1, &id);
		}
		virtual ~buffer() {
			glDeleteBuffers(1, &id);
		}

		buffer(const buffer&) = delete;
		buffer& operator=(const buffer&) = delete;

		buffer(buffer&& other) noexcept
			: id(std::exchange(other.id, 0)), target(other.target), usage(other.usage), size(other.size) {}
		buffer& operator=(buffer&& other) noexcept {
			if (this != &other) {
				if (id) glDeleteBuffers(1, &id);
				id = std::exchange(other.id, 0);
				target = other.target;
				usage = other.usage;
				size = other.size;
			}
			return *this;
		}

		void bind() {
			glBindBuffer(target, id);
		}

		void allocate(size_t size_byte, const void* data = nullptr) {
			bind();
			glBufferData(target, size_byte, data, usage);
			size = size_byte;
			capacity = size_byte;
		}

		void set_sub_data(size_t size_byte, const void* data, size_t offset = 0) {
			bind();

			// если данных больше чем зарезервировано, увеличиваем буффер в полтора раза
			if (capacity < offset + size_byte) {
				allocate((offset + size_byte) * 1.5);
				std::cerr << "warning: data's delete!\n";
			}
			else {
				size = size_byte;
			}

			glBufferSubData(target, offset, size_byte, data);
		}

		void get_sub_data(size_t size_byte, void* out_data, size_t offset = 0) {
			bind();

			if (size_byte + offset > capacity) {
				std::cerr << "this container are smaller than buffer\n";
				return;
			}

			glGetBufferSubData(target, offset, size_byte, out_data);
		}

		GLuint get_id() const { return id; }
	};
}