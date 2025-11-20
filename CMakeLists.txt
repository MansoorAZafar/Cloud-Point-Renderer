cmake_minimum_required(VERSION "3.26.0")
project("CloudPointRenderer" VERSION 1.0.0)

cmake_policy(SET CMP0072 NEW)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")


add_library(GLFW3 STATIC IMPORTED)
set_target_properties(GLFW3 PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/lib/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/include"
)


add_library(glad STATIC dependencies/externals/glad.c)
target_include_directories(glad PUBLIC dependencies/include)

file(GLOB MY_SOURCES "src/*.cpp")
add_executable("${CMAKE_PROJECT_NAME}" "${MY_SOURCES}")

target_include_directories("${CMAKE_PROJECT_NAME}" PUBLIC 
    "${PROJECT_SOURCE_DIR}/include"
)

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE 
    dependencies/include
    dependencies/include/glm    
)

find_package(OpenGL REQUIRED)
target_link_libraries("${CMAKE_PROJECT_NAME}" glad GLFW3 OpenGL::GL)