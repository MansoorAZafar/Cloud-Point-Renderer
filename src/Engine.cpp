#include "Engine.hpp"

pcr::Engine::Engine(const int& width, const int& height)
	: width(width), height(height), window(nullptr) { }

pcr::Engine::Engine(const char* filename, const int& density, const int& width, const int& height)
	: width(width), height(height), window(nullptr) {
	this->InitializeCloudBuffer(filename, density);
}


void pcr::Engine::Framebuffer_Size_Callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void pcr::Engine::InitializeOpenGL() {
	if (!glfwInit()) {
		throw std::runtime_error("Failed to initialize glfw, [ void pcr::Engine::InitializeOpenGL() ]\n");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(this->width, this->height, ENGINE_CONSTANTS::PCR_ENGINE_OPENGL_TITLE, NULL, NULL);
	if (!this->window) {
		throw std::runtime_error("Failed to create window, [ void pcr::Engine::InitializeOpenGL() ]\n");
	}

	glfwMakeContextCurrent(this->window);
	glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		throw std::runtime_error("Failed to initialize glad, [ void pcr::Engine::InitializeOpenGL() ]\n");
	}

	glViewport(0, 0, this->width, this->height);
	glfwSetFramebufferSizeCallback(this->window, Framebuffer_Size_Callback);
}


void pcr::Engine::InitializeBuffers(std::vector<glm::vec3>& points) {
	this->vao = std::make_unique<VertexArrayObject>();
	this->vao->Bind();

	this->vbo = std::make_unique<VertexBufferObject<glm::vec3>>(points);
	this->vbo->Bind();

	//VertexBufferObject<glm::vec3> pointsBuffer(points);
	this->vao->LinkToVAO<glm::vec3>(0u, 3u, this->vbo.get());
}


void pcr::Engine::InitializeShaders() {
	this->program = std::make_unique<ShaderProgram>(
		ENGINE_CONSTANTS::BASIC_BLOCK_VERTEX_FILE, 
		ENGINE_CONSTANTS::BASIC_BLOCK_FRAGMENT_FILE
	);
}


void pcr::Engine::ProcessInput(GLFWwindow* window, bool& pause) {
	if (!pause) {
		// game is running normally
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			pause = true;
		}
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
			pause = true; // skip potential rendering of the current iteration
			
			return;
		}
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			pause = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}

void pcr::Engine::InitializeCloudBuffer(const char* filename, const int& density) {
	CloudPointGenerator generator;
	generator.SetDensity(density);	
	this->asyncFileTask = generator.AsyncGetCloudPointsFromFile(filename);
}


void pcr::Engine::DrawGL() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_CULL_FACE);

	// If you load with asyncReadFile, there is no point in adding the call here
	//  - but keeping it in-case, don't want or didn't call async read file
	CloudPointGenerator generator;
	
	float prevTime = static_cast<float>(glfwGetTime());
	float deltaTime = 0.0f;
	
	bool firstMouseClick = true;
	bool pause = false;
	
	this->asyncFileTask.get();
	CloudPointData data = generator.GenerateCloudPoints("@Deprecated");
	std::future<void> _ = std::async(std::launch::async, [&generator] {
		// cleanup the generator asynchronously
		generator.FreeStaticFileData();
	});

	this->InitializeBuffers(data.points);
	this->InitializeShaders();
	Camera camera{ this->width, this->height, glm::vec3(0.0f) };

	while (!glfwWindowShouldClose(this->window)) {
		glfwPollEvents();
		this->ProcessInput(this->window, pause);

		if (!pause) {
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->BindAttributesDuringDraw();
			this->PaintGL(
				camera,
				deltaTime,
				prevTime,
				firstMouseClick,
				data.length
			);

			glfwSwapBuffers(this->window);
		}
		else {
			firstMouseClick = true;
		}
	}
}


void pcr::Engine::PaintGL(
	Camera& camera, 
	float& deltaTime, 
	float& prevTime, 
	bool& firstMouseClick, 
	size_t& length
) {
	glfwGetFramebufferSize(window, &this->width, &this->height);
	camera.UpdateScreenSize(this->width, this->height);

	float crntTime = static_cast<float>(glfwGetTime());
	deltaTime += static_cast<float>((crntTime - prevTime) / ENGINE_CONSTANTS::FPS_LIMIT);
	prevTime = crntTime;

	while (deltaTime >= 1) {
		camera.HandleInput(this->window, deltaTime, firstMouseClick);
		--deltaTime;
	}

	camera.Controller(this->program.get(), ENGINE_CONSTANTS::FOV);
	glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(length));
}


void pcr::Engine::BindAttributesDuringDraw() {
	this->vao->Bind();
	this->program->Bind();
}


void pcr::Engine::Show() {
	this->InitializeOpenGL();
	this->DrawGL();
}


pcr::Engine::~Engine() { 
	this->program->Delete();
	this->vbo->Delete();
	this->vao->Delete();

	glfwDestroyWindow(this->window);
	glfwTerminate();
}
