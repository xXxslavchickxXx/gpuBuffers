#pragma once

#include <IndexedBufferObject.h>

namespace ag {
	class uniform_buffer : public indexed_buffer_object {
    public:
        uniform_buffer(GLenum draw_mode = GL_DYNAMIC_DRAW)
            : indexed_buffer_object(GL_UNIFORM_BUFFER, draw_mode) {}
	};
}