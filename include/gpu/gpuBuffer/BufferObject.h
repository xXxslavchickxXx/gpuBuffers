#pragma once

#include <Buffer.h>

namespace ag {
	class buffer_object : public buffer {
		GLint bindingPoint = -1;

	protected:
		buffer_object(GLenum buffer_target, GLenum draw_mode = GL_DYNAMIC_DRAW)
			: buffer(buffer_target, draw_mode) {}

	public:
		void bind_base(GLint binding) {
			bindingPoint = binding;
			glBindBufferBase(target, bindingPoint, get_id());
		}

		template<typename T>
		void upload(const T* data, size_t count) {
			upload_raw(data, sizeof(T) * count);
		}

		template<concepts::Container T>
		void upload(const T& container) {
			size_t element_byte_size = sizeof(typename T::value_type);
			upload_raw(container.data(), element_byte_size * container.size());
		}

		template<concepts::TriviallyCopyable T>
		void upload(const T& scalar) {
			upload_raw(&scalar, sizeof(T));
		}

		template<typename T>
		void upload_part(const T* data, size_t count, size_t element_offset = 0) {
			upload_part_raw(data, sizeof(T) * count, sizeof(T) * element_offset);
		}
		template<concepts::TriviallyCopyable T>
		void upload_part(const T& scalar, size_t byte_offset = 0) {
			upload_part_raw(&scalar, sizeof(T), byte_offset);
		}

		template<concepts::Container T>
		void upload_part(const T& container, size_t element_offset = 0) {
			size_t element_byte_size = sizeof(typename T::value_type);
			upload_part_raw(container.data(), element_byte_size * container.size(), element_byte_size * element_offset);
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

		GLint get_binding() const {
			return bindingPoint;
		}

	private:
		void upload_part_raw(const void* data, size_t byte_size, size_t byte_offset = 0) {
			set_sub_data(byte_size, data, byte_offset);
		}
		void upload_raw(const void* data, size_t byte_size) {
			allocate(byte_size, data);
		}

	};
}