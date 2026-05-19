#pragma once

#include <span>
#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <vertex/vertexBuffer.hpp>
#include <vertex/indexedBuffer.hpp>

namespace engine::gpu {
	class vertex_array {
		GLuint vao = 0;
		// Карта где ключ - локация атрибута
		std::unordered_map<GLuint, std::unique_ptr<vertex_buffer>> buffers;
		std::unique_ptr<indexed_buffer> indexBuffer;

		static inline GLuint currentVAO = 0;

	public:
		vertex_array() {
			glewInit();
			glGenVertexArrays(1, &vao);
		}
		~vertex_array() { glDeleteVertexArrays(1, &vao); }

		vertex_buffer* operator[](size_t location) {
			if (buffers.find(location) == buffers.end()) {
				throw std::out_of_range("this location doesn't set\n");
			}
			return buffers[location].get();
		}

		void bind() {
			if (currentVAO != vao) {
				glBindVertexArray(vao);
				currentVAO = vao;
			}
		}

		vertex_buffer* addBuffer(GLenum mode, GLint attribIndex, GLint compCount, GLint valueSize) {
			if (buffers.find(attribIndex) != buffers.end()) {
				std::cerr << "this location is set\n";
			}

			bind();
			auto buffer = std::make_unique<vertex_buffer>(mode);
			buffer->bind();
			glVertexAttribPointer(attribIndex, compCount, GL_FLOAT, GL_FALSE, valueSize, 0);
			glEnableVertexAttribArray(attribIndex);

			auto ptr = buffer.get();
			buffers[attribIndex] = std::move(buffer);

			return ptr;
		}
		template<typename T>
		vertex_buffer* addBuffer(GLenum drawMode, GLint attribIndex) {
			return addBuffer(drawMode, attribIndex, type_to_gltype_c<T>, sizeof(T));
		}

		indexed_buffer* addIndexBuffer(GLenum drawMode) {
			bind();
			indexBuffer = std::make_unique<indexed_buffer>(drawMode);
			indexBuffer->bind();

			return indexBuffer.get();
		}

		indexed_buffer* getIndexBuffer() const { return indexBuffer.get(); }
	};
}