// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/

#include <iostream>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

int main()
{
     // Инициализируем GLFW
    if(!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4); // 4x сглаживание
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //нам не нужен старый OpenGL 

     // Откроем окно и создадим контекст
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Lesson 01", NULL, NULL);
    if(!window)
    {
        std::cerr << "Failed to open GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

     // Инициализируем GLEW
    glewExperimental = true; // Флаг необходим в Core-режиме OpenGL
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do {
        // Ничего не рисуем !
        // Меняем буффер
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); // Проверяем Escape на нажатие, или пользователь вручную закрыл окно
}