#ifndef PCR_SHADER_PROGRAM_HPP
#define PCR_SHADER_PROGRAM_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "InterfaceVertexObject.hpp"
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <future>

namespace pcr {

	class ShaderProgram: public InterfaceVertexObject {
	private:
		unsigned int CreateShader(const std::string& source, const unsigned int& type);
		std::string LoadShaderSource(const char* filepath);

	public:
		ShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile);

		void Bind() override;
		void Unbind() override;
		void Delete() override;

		unsigned int GetUniformLocationFromProgram(const char* uniformName);
		~ShaderProgram();
	};

};

#endif

