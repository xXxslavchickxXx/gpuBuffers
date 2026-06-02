#pragma once

#include <IndexedBufferObject.h>

namespace ag {
    class indirect_buffer : public buffer_object {
    public:
        indirect_buffer(GLenum draw_mode = GL_DYNAMIC_DRAW)
            : buffer_object(GL_DRAW_INDIRECT_BUFFER, draw_mode) {}
    };
}