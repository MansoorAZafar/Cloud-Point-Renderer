#include "VertexArrayObject.hpp"

pcr::VertexArrayObject::VertexArrayObject() {
	glGenVertexArrays(BUFFER_CONSTANTS::SINGLE_BUFFER, &this->ID);
	this->Bind();
}


void pcr::VertexArrayObject::Bind() {
	glBindVertexArray(this->ID);
}


void pcr::VertexArrayObject::Unbind() {
	glBindVertexArray(BUFFER_CONSTANTS::CLEAR_BUFFER);
}


void pcr::VertexArrayObject::Delete() {
	glDeleteVertexArrays(BUFFER_CONSTANTS::SINGLE_BUFFER, &this->ID);
}


pcr::VertexArrayObject::~VertexArrayObject() {
	if (this->ID) {
		this->Delete();
	}
}
