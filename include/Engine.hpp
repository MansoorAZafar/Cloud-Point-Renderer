#ifndef PCR_ENGINE_HPP
#define PCR_ENGINE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <stdexcept>
#include <memory>

#include "CloudPointGenerator.hpp"
#include "VertexBufferObject.hpp"
#include "VertexArrayObject.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"

namespace pcr {
	
	namespace ENGINE_CONSTANTS {

		static constexpr const char* PCR_ENGINE_OPENGL_TITLE   = "Point Cloud Renderer";
		static constexpr const char* BASIC_BLOCK_VERTEX_FILE   = "Shaders/point.vert";
		static constexpr const char* BASIC_BLOCK_FRAGMENT_FILE = "Shaders/point.frag";
		static constexpr double       FPS_LIMIT                = (1.0 / 60.0);
		static constexpr float        FOV                      = 60.0;
	};

	/*
	*	Manages the lifetime of all OpenGL objects and rendering processes
	* l---------------------------------------------l
	*	@TODO:
	*		- Singleton, one only instance
	* l---------------------------------------------l
	*	@MVP
	*		1. Have a Window						: DONE
	*		2. Render Point Cloud of PreSet object	: DONE
	*		3. Be able to Move around World			: DONE
	*/
	class Engine {
	private:

		std::unique_ptr<VertexBufferObject<glm::vec3>> vbo{};
		std::unique_ptr<VertexArrayObject> vao{};
		std::unique_ptr<ShaderProgram> program{};
		std::future<void> asyncFileTask;

		GLFWwindow* window;
		int width;
		int height;


		static void Framebuffer_Size_Callback(GLFWwindow* window, int width, int height);
		void PaintGL(
			Camera& camera, 
			float& deltaTime, 
			float& prevTime, 
			bool& firstMouseClick, 
			size_t& length
		);

		void InitializeBuffers(std::vector<glm::vec3>& points);
		void ProcessInput(GLFWwindow* window, bool& pause);
		void InitializeCloudBuffer(const char* filename);
		void BindAttributesDuringDraw();
		void InitializeShaders();
		void InitializeOpenGL();
		void DrawGL();

	public:
		Engine(const int& width = 640, const int& height = 480);
		Engine(const char* filename, const int& width = 640, const int& height = 480);

		void Show();
		~Engine();
	};
}


#endif
