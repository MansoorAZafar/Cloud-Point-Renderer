#include "ShaderProgram.hpp"

pcr::ShaderProgram::ShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile) {
    this->ID = glCreateProgram();

    std::future<std::string> vertexShaderSource
        = std::async(std::launch::async, &ShaderProgram::LoadShaderSource, this, vertexShaderFile);
    std::future<std::string> fragmentShaderSource
        = std::async(std::launch::async, &ShaderProgram::LoadShaderSource, this, fragmentShaderFile);

    const std::string vertexSource = vertexShaderSource.get();
    const std::string fragmentSource = fragmentShaderSource.get();

    unsigned int vertexShader = this->CreateShader(vertexSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = this->CreateShader(fragmentSource, GL_FRAGMENT_SHADER);

    glAttachShader(this->ID, vertexShader);
    glAttachShader(this->ID, fragmentShader);

    glLinkProgram(this->ID);
    glValidateProgram(this->ID);

    glDetachShader(this->ID, vertexShader);
    glDetachShader(this->ID, fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


unsigned int pcr::ShaderProgram::CreateShader(const std::string& source, const unsigned int& type) {
    unsigned int shader = glCreateShader(type);
    const char* rawSrc = source.c_str();
    glShaderSource(shader, 1, &rawSrc, nullptr);
    glCompileShader(shader);

    int result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

        char* message = (char*)_malloca(sizeof(char) * length);
        glGetShaderInfoLog(shader, length, &length, message);

        std::cerr << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")
            << " Shader\n"
            << message
            << "\n";

        _freea(message);
        throw std::runtime_error("Failed to compile shader");
    }

    return shader;
}


std::string pcr::ShaderProgram::LoadShaderSource(const char* filepath) {
    std::ifstream ifs(filepath);
    if (!ifs.is_open()) {
        throw std::runtime_error("File could not be opened: " 
            + std::string(filepath) + "[ShaderProgram::LoadShaderSource]\n");
    }
    
    return static_cast<std::stringstream const&>(std::stringstream() << ifs.rdbuf()).str();
}


void pcr::ShaderProgram::Bind() {
    glUseProgram(this->ID);
}


void pcr::ShaderProgram::Unbind() {
    glUseProgram(BUFFER_CONSTANTS::CLEAR_BUFFER);
}


void pcr::ShaderProgram::Delete() {
    glDeleteProgram(this->ID);
}


unsigned int pcr::ShaderProgram::GetUniformLocationFromProgram(const char* uniformName) {
    return glGetUniformLocation(this->ID, uniformName);
}


pcr::ShaderProgram::~ShaderProgram() {
    if (this->ID) {
        this->Delete();
    }
}
