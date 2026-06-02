#pragma once

#include <BufferObject.h>

namespace ag {
	class uniform_buffer : public buffer_object {
    public:
        uniform_buffer(GLenum draw_mode = GL_DYNAMIC_DRAW)
            : buffer_object(GL_UNIFORM_BUFFER, draw_mode) {}

        static void bind_block(GLuint program, const std::string& blockName, GLint binding) {
            GLuint blockIndex = glGetUniformBlockIndex(program, blockName.c_str());
            if (blockIndex != GL_INVALID_INDEX) {
                glUniformBlockBinding(program, blockIndex, binding);
            }
        }
	};
}