#include "Camera.hpp"

pcr::Camera::Camera(int& width, int& height, glm::vec3&& startingPosition)
	: width(width), height(height), pitch(0.0f), yaw(-90.0f) {
	
	this->up    = glm::vec3(0.0f, 1.0f, 0.0f);
	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->right = glm::vec3(1.0f, 0.0f, 0.0f);
	
	this->yaw = -90.0f;
	this->pitch = 0.0f;

	this->lastX = static_cast<float>(width / 2.0);
	this->lastY = static_cast<float>(height / 2.0);
	this->cameraPosition = std::move(startingPosition);
}


void pcr::Camera::HandleInput(GLFWwindow* input, const float& delta, bool& firstMouseClick) {
	this->InputController(input, delta, firstMouseClick);
}


void pcr::Camera::InputController(GLFWwindow* input, const float& delta, bool& firstMouseClick) {
	if (glfwGetInputMode(input, GLFW_CURSOR) != GLFW_CURSOR_DISABLED)
		return;

	if (glfwGetKey(input, GLFW_KEY_EQUAL) == GLFW_PRESS) {
		this->IncreaseSpeed(CAMERA_CONSTANTS::INCREASE_SPEED);
	} else if (glfwGetKey(input, GLFW_KEY_MINUS) == GLFW_PRESS) {
		this->IncreaseSpeed(CAMERA_CONSTANTS::DECREASE_SPEED);
	}	

	const float cameraSpeed = static_cast<float>(this->speed * delta) / 2.0f;
	glm::vec3 flatFront = glm::normalize(glm::vec3(front.x, 0.0f, front.z));

	if (glfwGetKey(input, GLFW_KEY_W)) {
		this->cameraPosition += cameraSpeed * flatFront;
	}
	if (glfwGetKey(input, GLFW_KEY_A)) {
		this->cameraPosition -= cameraSpeed * this->right;
	}
	if (glfwGetKey(input, GLFW_KEY_S)) {
		this->cameraPosition -= cameraSpeed * flatFront;
	}
	if (glfwGetKey(input, GLFW_KEY_D)) {
		this->cameraPosition += cameraSpeed * this->right;
	}

	if (glfwGetKey(input, GLFW_KEY_SPACE)) {
		this->cameraPosition.y += cameraSpeed;
	}
	if (glfwGetKey(input, GLFW_KEY_LEFT_SHIFT)) {
		this->cameraPosition.y -= cameraSpeed;
	}
	
	double xpos;
	double ypos;

	glfwGetCursorPos(input, &xpos, &ypos);
	this->UpdateCameraVectors(firstMouseClick, xpos, ypos);
}


void pcr::Camera::UpdateCameraVectors(bool& firstMouseClick, double xpos, double ypos) {
	if (firstMouseClick) {
		this->lastX = static_cast<float>(xpos);
		this->lastY = static_cast<float>(ypos);
		firstMouseClick = false;
	}

	float xoffset = static_cast<float>(xpos - this->lastX);
	float yoffset = static_cast<float>(ypos - this->lastY);
	this->lastX = static_cast<float>(xpos);
	this->lastY = static_cast<float>(ypos);

	constexpr float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	this->yaw += xoffset;
	this->pitch -= yoffset;

	if (this->pitch > 89.0f) {
		this->pitch = 89.0f;
	}
	else if (this->pitch < -89.0f) {
		this->pitch = -89.0f;
	}

	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	this->front = glm::normalize(direction);
	this->right = glm::normalize(glm::cross(this->front, CAMERA_CONSTANTS::UnitY));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}


void pcr::Camera::Controller(
	ShaderProgram* shader, 
	float FOVdegree, 
	const float& nearPlane, 
	const float& farPlane,
	const float& rotation
) {
	glm::mat4 model      = CAMERA_CONSTANTS::IDENTITY_MATRIX;
	glm::mat4 view       = this->GetViewMatrix();
	glm::mat4 projection = this->GetProjectionMatrix(FOVdegree, nearPlane, farPlane);

	model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 1.0f, 0.0f));
	view  = glm::translate(view, glm::vec3(0.0f, -1.0f, -30.0f));


	const unsigned int modelLocation      = shader->GetUniformLocationFromProgram("model"     );
	const unsigned int viewLocation       = shader->GetUniformLocationFromProgram("view"      );
	const unsigned int projectionLocation = shader->GetUniformLocationFromProgram("projection");

	glUniformMatrix4fv(modelLocation     , 1, GL_FALSE, glm::value_ptr(model     ));
	glUniformMatrix4fv(viewLocation      , 1, GL_FALSE, glm::value_ptr(view      ));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
}


inline glm::mat4 pcr::Camera::GetViewMatrix() const {
	return glm::lookAt(this->cameraPosition, this->cameraPosition + this->front, this->up);
}


inline glm::mat4 pcr::Camera::GetProjectionMatrix(const float& FOVdegree,
	const float& nearPlane,
	const float& farPlane
) const {
	return glm::perspective(
		glm::radians(FOVdegree),
		(static_cast<float>(this->width) / this->height),
		nearPlane,
		farPlane
	);
}


void pcr::Camera::UpdateScreenSize(const int& width, const int& height) {
	this->width = width;
	this->height = height;
}

void pcr::Camera::IncreaseSpeed(const float& speed) {
	const float increasedSpeed = this->speed + speed;
	if(increasedSpeed >= this->MIN_SPEED && increasedSpeed <= this->MAX_SPEED ) { 
		this->speed = increasedSpeed; 
	}
}

