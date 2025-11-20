#ifndef PCR_VERTEX_ARRAY_OBJECT_HPP
#define PCR_VERTEX_ARRAY_OBJECT_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InterfaceVertexObject.hpp"
#include "VertexBufferObject.hpp"

namespace pcr {
	/*
	* Represents a VAO, stores VBO's for rendering
	*/
	class VertexArrayObject: public InterfaceVertexObject {
	public:
		VertexArrayObject();

		void Bind() override;
		void Unbind() override;
		void Delete() override;
		
		template <class T>
		void LinkToVAO(unsigned int location, 
			unsigned int size, 
			VertexBufferObject<T>* vbo,
			const unsigned int& stride = 0,
			const void* offset = 0
		) {
			this->Bind();
			vbo->Bind();

			glEnableVertexAttribArray(location);
			glVertexAttribPointer(location, size, GL_FLOAT, GL_FALSE, stride, offset);
			
			this->Unbind();
		}
		
		~VertexArrayObject();
	};
}

#endif

