// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

#include <iostream>
#include <optional>

#define FREEGLUT_STATIC
#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"

int HEIGHT = 1024;
int WIDTH  = 768;

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
    GLFWwindow* window = glfwCreateWindow(HEIGHT, WIDTH, "Lesson 02", NULL, NULL);
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

    std::optional<GLuint> programIDOptional = loadShaders("simpleShader.vert", "simpleShader.frag");

    if(!programIDOptional)
    {
        return EXIT_FAILURE;
    }

    GLuint programID = programIDOptional.value();

    // Матрица проекции: поле вида 45°, соотношение сторон, диапазон отображения 0.1 unit <-> 100 units
    glm::mat4 projectionMat4 = glm::perspective(glm::radians(45.0f), static_cast<float>(WIDTH) / static_cast<float>(HEIGHT), 0.1f, 100.0f);

    // Или для ортогональной камеры:
    // glm::mat4 projectionMat4 = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.0f, 100.0f); // В мировых координатах

    //Матрица камеры
    glm::mat4 viewMat4 = glm::lookAt(
        glm::vec3(4, 3, 3), // Координаты камеры в мировом пространстве
        glm::vec3(0, 0, 0), // и куда смотрит
        glm::vec3(0, 1, 0)  // Видимо с какой стороны находится верх камеры (0,-1,0 для вида вверх ногами)
    );

    // Матрица модели
    glm::mat4 modelMat4 = glm::mat4(1.0f);

    // Матрица МодельВидПроекция: перемножение трех матриц
    glm::mat4 mvp = projectionMat4 * viewMat4 * modelMat4;

    // Получение id для матрицы MVP в шейдерной программе
    GLuint mvpMatID = glGetUniformLocation(programID, "MVP");

    // Установка размера окна просмотра OpenGL
    glViewport(0, 0, WIDTH, HEIGHT);

    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);

        glBindVertexArray(vertexArrayID);

        // Отправка матрицы текущему привязаному шейдеру в MVP
        glUniformMatrix4fv(mvpMatID, 1, GL_FALSE, glm::value_ptr(mvp)); // последний аргумент указатель на массив

        // Первый буффер аттрибутов - вершины  
        glVertexAttribPointer(
            0, // значение для layout в шейдере
            3, // размер аттрибута. В данном случае вершины, которая состоит из 3х координат
            GL_FLOAT, // тип координат вершин в частности и тип данных аттрибута в общем
            GL_FALSE, // нужно ли нормализовать?
            0, // шаг между атрибутами вершин 
            nullptr // смещение того места где начинаются данные в буффере
        );
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 3); // начиная с вершины 0 нарисовать все 3 вершины которые образуют треугольник
        glDisableVertexAttribArray(0);

        // Меняем буффер
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); // Проверяем Escape на нажатие, или пользователь вручную закрыл окно
}