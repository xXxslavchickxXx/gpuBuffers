#pragma once

#include <span>
#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <traits.h>

namespace engine::gpu {
	class Buffer {
		GLuint id;
		GLenum target;
		GLenum usage;

		size_t elementSize = 0;
		size_t capacity = 0;
	
	public:
		Buffer(GLenum target, GLenum usage, size_t elementSize) : target(target), usage(usage), elementSize(elementSize)
		{
			glGenBuffers(1, &id);
		}
		~Buffer() {
			glDeleteBuffers(1, &id);
		}

		void upload(const void* data, GLuint counts) {
			// counts - количество вершин которых вы хотите за аплоадить
			size_t bytes = counts * elementSize;

			// если буффер меньше, то увеличиваем его в два раза
			glBindBuffer(target, id);
			if (capacity < bytes) {
				capacity = 2 * bytes;
				glBufferData(target, capacity, nullptr, usage);
			}
			glBufferSubData(target, 0, bytes, data);
		}

		template<typename Container>
		void upload(Container&& container) 
		requires (have_size_v<Container> && have_data_v<Container>)
		{
			upload(container.data(), container.size());
		}

		// Обновление каких то частей изначальной даты, первые два относятся к массиву
		// данных которые вы хотите добавить, а второй это индекс откуда начать
		void uploadPart(const void* data, GLuint count, GLuint offsetElements) {
			size_t bytes = count * elementSize;
			size_t offsetBytes = offsetElements * elementSize;

			if (offsetBytes + bytes > capacity) {
				throw std::runtime_error("Buffer overflow in uploadPart\n");
			}

			glBindBuffer(target, id);
			glBufferSubData(target, offsetBytes, bytes, data);
		}
		template<typename Container>
		void uploadPart(Container&& container, GLuint offsetElements)
			requires (have_size_v<Container> && have_data_v<Container>)
		{
			uploadPart(container.data(), container.size(), offsetElements);
		}

		GLuint getId() const { return id; }
	};

	class VertexArray {
		GLuint vao = 0;
		std::vector<std::unique_ptr<Buffer>> buffers;
		std::unique_ptr<Buffer> indexBuffer;

		static inline GLuint currentVAO = 0;

	public:
		VertexArray() {
			glewInit();
			glGenVertexArrays(1, &vao);
		}
		~VertexArray() { glDeleteVertexArrays(1, &vao); }

		void bind() {
			if (currentVAO != vao) {
				glBindVertexArray(vao);
				currentVAO = vao;
			}
		}

		Buffer* addBuffer(GLenum target, GLenum mode, GLenum type, GLint attribIndex, GLint compCount, GLint valueSize) {
			bind();
			auto buffer = std::make_unique<Buffer>(target, mode, valueSize);
			glBindBuffer(target, buffer->getId());
			glVertexAttribPointer(attribIndex, compCount, type, GL_FALSE, valueSize, 0);
			glEnableVertexAttribArray(attribIndex);

			auto ptr = buffer.get();
			buffers.push_back(std::move(buffer));

			return ptr;
		}
		template<typename T>
		Buffer* addBuffer(GLenum target, GLenum mode, GLint attribIndex) {
			return addBuffer(target, mode, type_to_gltype_t<T>, attribIndex, type_to_gltype_c<T>, sizeof(T));
		}

		Buffer* addIndexBuffer(GLenum mode) {
			bind();
			indexBuffer = std::make_unique<Buffer>(GL_ELEMENT_ARRAY_BUFFER, mode, sizeof(uint32_t));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer->getId());

			return indexBuffer.get();
		}

		Buffer* getIndexBuffer() const { return indexBuffer.get(); }
	};
}