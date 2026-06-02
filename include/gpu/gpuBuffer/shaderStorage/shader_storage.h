#pragma once

#include <IndexedBufferObject.h>

namespace ag {
    class shader_storage_buffer : public indexed_buffer_object {
    public:
        shader_storage_buffer(GLenum draw_mode = GL_DYNAMIC_DRAW)
            : indexed_buffer_object(GL_SHADER_STORAGE_BUFFER, draw_mode) {}
    };
}