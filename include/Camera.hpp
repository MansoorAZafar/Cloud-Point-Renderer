#ifndef PCR_CAMERA_HPP
#define PCR_CAMERA_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include "ShaderProgram.hpp"

namespace pcr {

	namespace CAMERA_CONSTANTS {
		static constexpr glm::mat4 IDENTITY_MATRIX    = glm::mat4(1.0f);
		static constexpr glm::vec3 UnitY              = glm::vec3(0.0f, 1.0f, 0.0f);

		static constexpr float     DEFAULT_NEAR_PLANE = 0.01f;
		static constexpr float     DEFAULT_FAR_PLANE  = 100.0f;

		static constexpr const float SPEED = 0.1f;
		static constexpr const float SENSITIVITY = 100.0f;

	}

	class Camera {
	private:

		glm::vec3 up;
		glm::vec3 front;
		glm::vec3 right;
		glm::vec3 cameraPosition;
		
		int width;
		int height;

		float pitch;
		float yaw;

		float lastX;
		float lastY;
		
		void InputController(GLFWwindow* input, const float& delta, bool& firstMouseClick);
		void UpdateCameraVectors(bool& firstMouseClick, double xpos, double ypos);

		glm::mat4 GetViewMatrix() const;
		glm::mat4 GetProjectionMatrix(
			const float& FOVdegree,
			const float& nearPlane = CAMERA_CONSTANTS::DEFAULT_NEAR_PLANE,
			const float& farPlane  = CAMERA_CONSTANTS::DEFAULT_FAR_PLANE
		) const;

	public:
		Camera(int& width, int& height, glm::vec3&& startingPosition);

		void Controller(
			ShaderProgram* shader,
			float FOVdegree,
			const float& nearPlane = CAMERA_CONSTANTS::DEFAULT_NEAR_PLANE,
			const float& farPlane  = CAMERA_CONSTANTS::DEFAULT_FAR_PLANE,
			const float& rotation  = 0.0f
		);

		void HandleInput(GLFWwindow* input, const float& delta, bool& firstMouseClick);
		void UpdateScreenSize(const int& width, const int& height);
	};

};

#endif

