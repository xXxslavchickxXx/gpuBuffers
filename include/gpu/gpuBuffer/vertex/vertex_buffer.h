#pragma once

#include <BufferObject.h>

namespace ag {
	namespace gpu {
		class vertex_array;
	}

	class vertex_buffer : public buffer_object {
		friend ag::gpu::vertex_array;

		GLint compCount = -1;
		GLint byteSize = -1;
		GLenum type = -1;
		GLboolean normalized = -1;

		bool is_init = false;

		void set_format(GLint compCount, GLint byteSize, GLenum type, GLboolean normalized) {
			this->compCount = compCount; this->byteSize = byteSize; this->type = type; this->normalized = normalized; is_init = true;
		}

	public:
		vertex_buffer(GLenum draw_mode = GL_DYNAMIC_DRAW) : buffer_object(GL_ARRAY_BUFFER, draw_mode) {}
	};
}