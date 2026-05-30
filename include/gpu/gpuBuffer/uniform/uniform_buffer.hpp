#pragma once

#include <buffer.hpp>

namespace ag {
	class uniform_buffer : public buffer {
		GLint bindingPoint = -1;

	public:
		using buffer::buffer;

	public:
		uniform_buffer() : buffer(GL_UNIFORM_BUFFER) {}
		uniform_buffer(GLenum draw_mode) : buffer(GL_UNIFORM_BUFFER, draw_mode) {}

		void bind_base(GLint binding) {
			bindingPoint = binding;
			glBindBufferBase(target, bindingPoint, get_id());
		}

		template<typename T>
		void upload(const T& data) {
			allocate(sizeof(T), &data);
		}

		template<typename T>
		void upload_array(const T* data, size_t count) {
			allocate(sizeof(T) * count, data);
		}

		template<typename T>
		void upload_part(const T& data, size_t offset = 0) {
			set_sub_data(sizeof(T), &data, offset);
		}

		template<typename T>
		void upload_part(const T* data, size_t count, size_t offset = 0) {
			set_sub_data(sizeof(T) * count, data, offset);
		}

		void download_part(size_t size_byte, void* out_data, size_t offset = 0) {
			get_sub_data(size_byte, out_data, offset);
		}

		template<typename T>
		void download_part(T& out_data, size_t offset = 0) {
			download_part(sizeof(T), &out_data, offset);
		}

		void bind_range(GLint binding, GLint left, GLint right) {
			if (id != GL_INVALID_INDEX) {
				glBindBufferRange(target, binding, id, left, right);
			}
		}

		static void bind_block(GLuint program, const std::string& blockName, GLint binding) {
			GLuint blockIndex = glGetUniformBlockIndex(program, blockName.c_str());
			if (blockIndex != GL_INVALID_INDEX) {
				glUniformBlockBinding(program, blockIndex, binding);
			}
		}

		GLint get_binding() const {
			return bindingPoint;
		}
	};
}