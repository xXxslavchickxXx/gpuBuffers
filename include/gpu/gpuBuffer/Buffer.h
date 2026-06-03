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

		void allocate(size_t new_capacity, const void* data = nullptr) {
			if (new_capacity <= capacity) {
				if (data) {
					bind();
					glBufferSubData(target, 0, new_capacity, data);
				}
				return;
			}

			GLuint s_temp_buffer = 0;
			glGenBuffers(1, &s_temp_buffer);
			glBindBuffer(GL_COPY_WRITE_BUFFER, s_temp_buffer);
			glBufferData(GL_COPY_WRITE_BUFFER, new_capacity, nullptr, usage);

			if (size > 0) {
				glBindBuffer(GL_COPY_READ_BUFFER, id);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, size);
			}

			bind();
			glBufferData(target, new_capacity, nullptr, usage);

			if (size > 0) {
				glBindBuffer(GL_COPY_READ_BUFFER, s_temp_buffer);
				glCopyBufferSubData(GL_COPY_READ_BUFFER, target, 0, 0, size);
			}

			glDeleteBuffers(1, &s_temp_buffer);

			if (data) {
				glBufferSubData(target, size, new_capacity - size, (const uint8_t*)data + size);
			}

			capacity = new_capacity;
		}

		void reserve(size_t new_capacity) {
			if (new_capacity <= capacity) return;
			allocate(new_capacity, nullptr);
		}

		void set_sub_data(size_t data_size, const void* data, size_t offset = 0) {
			size_t required = offset + data_size;

			if (required > capacity) {
				allocate((std::max)(required, capacity * 2), nullptr);
			}

			bind();
			glBufferSubData(target, offset, data_size, data);

			if (required > size) {
				size = required;
			}
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