#pragma once

#include <BufferObject.h>

namespace ag {
	class indexed_buffer_object : public buffer_object {
		GLint bindingPoint = -1;
		using buffer_object::buffer_object;

	public:
		static void bind_block(GLuint program, const std::string& blockName, GLint binding) {
			GLuint blockIndex = glGetUniformBlockIndex(program, blockName.c_str());
			if (blockIndex != GL_INVALID_INDEX) {
				glUniformBlockBinding(program, blockIndex, binding);
			}
		}

		void bind_base(GLint binding) {
			bindingPoint = binding;
			glBindBufferBase(target, bindingPoint, get_id());
		}
		void bind_range(GLint binding, GLint left, GLint right) {
			if (id != GL_INVALID_INDEX) {
				glBindBufferRange(target, binding, id, left, right);
			}
		}

		GLint get_binding() const {
			return bindingPoint;
		}
	};
}