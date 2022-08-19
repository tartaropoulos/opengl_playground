cmake_minimum_required(VERSION 3.15.5)

project(OpenglPlayground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_subdirectory(src/opengl_tutorial/lesson_1)
add_subdirectory(src/opengl_tutorial/lesson_2)
add_subdirectory(src/opengl_tutorial/lesson_3)
add_subdirectory(src/opengl_tutorial/lesson_4)
add_subdirectory(src/opengl_tutorial/lesson_4_one_buffer)
add_subdirectory(src/opengl_tutorial/lesson_5)