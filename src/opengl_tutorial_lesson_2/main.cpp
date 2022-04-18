// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

#include <iostream>
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

#include "shader.h"

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
    GLFWwindow* window = glfwCreateWindow(1024, 768, "Lesson 02", NULL, NULL);
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

    // Темно синий бэкграунд
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Настройка VAO
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    static const GLfloat gVertexBufferData[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Настройка VBO
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Перадеатся буффер в OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

    GLuint programID = loadShaders("simpleShader.vert", "simpleShader.frag");

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        // Первый буффер аттрибутов - вершины
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0, // значение для layout в шейдере
            3, // размер аттрибута. В данном случае вершины, которая состоит из 3х координат
            GL_FLOAT, // тип координат вершин в частности и тип данных аттрибута в общем
            GL_FALSE, // нужно ли нормализовать?
            0, // stride wtf?
            nullptr // не точно, но возможно смещение в буффере
        );

        glDrawArrays(GL_TRIANGLES, 0, 3); // начиная с вершины 0 нарисовать все 3 вершины которые образуют треугольник
        glDisableVertexAttribArray(0);

        // Ничего не рисуем !
        // Меняем буффер
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); // Проверяем Escape на нажатие, или пользователь вручную закрыл окно
}