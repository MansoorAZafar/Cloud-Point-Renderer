#ifndef PCR_VERTEX_BUFFER_OBJECT_HPP
#define PCR_VERTEX_BUFFER_OBJECT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "InterfaceVertexObject.hpp"

namespace pcr {

	/*
	* Represents a VBO, holds data about the current property being rendered
	*/
	template<class T >
	class VertexBufferObject: public InterfaceVertexObject {
	private:
		void GenerateBuffer(const unsigned int& size, const void* data) {
			glGenBuffers(BUFFER_CONSTANTS::SINGLE_BUFFER, &this->ID);
			this->Bind();
			glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
		}
	public:
		VertexBufferObject(std::vector<T>& points) {
			const unsigned int size = static_cast<unsigned int>(points.size() * sizeof(T));
			const void* _data = points.data();

			this->GenerateBuffer(size, _data);
		}

		void Bind() {
			glBindBuffer(GL_ARRAY_BUFFER, this->ID);
		}

		void Unbind() {
			glBindBuffer(GL_ARRAY_BUFFER, BUFFER_CONSTANTS::CLEAR_BUFFER);
		}

		void Delete() {
			glDeleteBuffers(BUFFER_CONSTANTS::SINGLE_BUFFER, &this->ID);
		}

		~VertexBufferObject() {
			if (this->ID) {
				this->Delete();
			}
		}

	};

}
#endif

