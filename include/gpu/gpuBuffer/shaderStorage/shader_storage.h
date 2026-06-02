#pragma once

#include <BufferObject.h>

namespace ag {
    class shader_storage_buffer : public buffer_object {
    public:
        shader_storage_buffer(GLenum draw_mode = GL_DYNAMIC_DRAW)
            : buffer_object(GL_SHADER_STORAGE_BUFFER, draw_mode) {}

        static void bind_block(GLuint program, const std::string& blockName, GLint binding) {
            GLuint blockIndex = glGetProgramResourceIndex(program, GL_SHADER_STORAGE_BLOCK, blockName.c_str());
            if (blockIndex != GL_INVALID_INDEX) {
                glShaderStorageBlockBinding(program, blockIndex, binding);
            }
        }
    };
}