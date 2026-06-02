#pragma once

#include <span>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <unordered_map>

#include <vertex/vertex_buffer.h>
#include <vertex/indexed_buffer.h>
#include <vertex/vertex_weak_ref.h>

namespace ag::gpu {
	class vertex_array {
		GLuint vao = 0;

		std::unordered_map<GLuint, std::shared_ptr<vertex_buffer>> buffers;
		std::unique_ptr<indexed_buffer> indexBuffer;

		static inline GLuint currentVAO = 0;

	public:
		vertex_array() {
			glGenVertexArrays(1, &vao);
		}
		~vertex_array() { glDeleteVertexArrays(1, &vao); }

		vertex_buffer_ref operator[](size_t location) {
			if (buffers.find(location) == buffers.end()) {
				throw std::out_of_range("this location doesn't set\n");
			}
			return buffers[location];
		}

		void bind() {
			if (currentVAO != vao) {
				glBindVertexArray(vao);
				currentVAO = vao;
			}
		}

		vertex_buffer_ref add_buffer(GLenum mode, GLint attribIndex, GLint compCount, GLint valueSize) {
			if (buffers.find(attribIndex) != buffers.end()) {
				std::cerr << "this location is set\n";
			}

			bind();
			auto buffer = std::make_shared<vertex_buffer>(mode);
			buffer->bind();
			glVertexAttribPointer(attribIndex, compCount, GL_FLOAT, GL_FALSE, valueSize, 0);
			glEnableVertexAttribArray(attribIndex);

			buffers[attribIndex] = std::move(buffer);
			return buffers[attribIndex];
		}
		template<typename T>
		vertex_buffer_ref add_buffer(GLenum drawMode, GLint attribIndex) {
			return add_buffer(drawMode, attribIndex, type_to_gltype_c<T>, sizeof(T));
		}

		void set_buffer(GLint location, std::shared_ptr<vertex_buffer> buffer) {
			auto it = buffers.find(location);
			if (it == buffers.end()) {
				throw std::out_of_range("Location not set");
			}

			auto& existing = it->second;

			if (!buffer->is_init) {
				buffer->set_format(existing->compCount, existing->byteSize,
					existing->type, existing->normalized);
			}
			else {
				if (buffer->compCount != existing->compCount ||
					buffer->byteSize != existing->byteSize ||
					buffer->type != existing->type ||
					buffer->normalized != existing->normalized) {
					throw std::runtime_error("Buffer format mismatch!");
				}
			}

			buffers[location] = buffer;

			bind();
			buffer->bind();
			glVertexAttribPointer(location, buffer->compCount, buffer->type,
				buffer->normalized, buffer->byteSize, 0);
			glEnableVertexAttribArray(location);
		}

		indexed_buffer* add_index_buffer(GLenum drawMode) {
			bind();
			indexBuffer = std::make_unique<indexed_buffer>(drawMode);
			indexBuffer->bind();

			return indexBuffer.get();
		}

		indexed_buffer* get_index_buffer() const { return indexBuffer.get(); }
	};
}