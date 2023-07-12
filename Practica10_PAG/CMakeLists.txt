cmake_minimum_required(VERSION 3.20)
project(Practica10_PAG)

set(CMAKE_CXX_STANDARD 14)

find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)


add_executable(Practica10_PAG main.cpp Renderer.cpp Renderer.h Shader.cpp Shader.h Model.cpp Model.h Material.cpp Material.h Camera.cpp Camera.h Light.cpp Light.h lodepng.h lodepng.cpp)

link_directories( /usr/local/include/ )

target_link_libraries( Practica10_PAG OpenGL::GL glfw GLEW::glew assimp::assimp)
