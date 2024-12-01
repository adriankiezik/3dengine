cmake_minimum_required(VERSION 3.10)

# Project name and C++ standard
project(OpenGLApp)
set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(include)
include_directories(lib/imgui)  # Add ImGui include directory first
include_directories(lib/imgui/backends)  # Add ImGui backend directory

# Add the glad source file
add_library(glad src/glad.c)

# Find OpenGL and GLFW
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Find all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")

# Add ImGui source files
file(GLOB IMGUI_SOURCES "lib/imgui/*.cpp")
file(GLOB IMGUI_BACKEND_SOURCES "lib/imgui/backends/*.cpp")  # Add backend files

# Create a single executable (e.g., OpenGLApp)
add_executable(OpenGLApp ${SOURCES} ${IMGUI_SOURCES} ${IMGUI_BACKEND_SOURCES})

# Link libraries
target_link_libraries(OpenGLApp glad glfw ${OPENGL_LIBRARIES})
