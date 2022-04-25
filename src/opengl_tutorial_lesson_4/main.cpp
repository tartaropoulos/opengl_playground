// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-4-a-colored-cube/

#include <iostream>
#include <optional>

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

    // Включение теста глубины (Z-буффер)
    glEnable(GL_DEPTH_TEST);

    // Отображается ближайший треугольник к камере
    glDepthFunc(GL_LESS);

    // Настройка VAO
    GLuint vertexArrayID;
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    static const GLfloat gVertexBufferData[] = {
        -0.5f,-0.5f,-0.5f, // triangle 1 : begin
        -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, // triangle 1 : end
         0.5f, 0.5f,-0.5f, // triangle 2 : begin
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f, // triangle 2 : end
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f
    };

    // Настройка VBO
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Перадеатся буффер в OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

    static const GLfloat gColorBufferData[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gColorBufferData), gColorBufferData, GL_STATIC_DRAW);

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
        glm::vec3(4, 3, -3), // Координаты камеры в мировом пространстве
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

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
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

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        // Второй буффер аттрибутов - цвета
        glVertexAttribPointer(
            1, // значение для layout в шейдере
            3, // размер аттрибута. В данном случае вершины, которая состоит из 3х координат
            GL_FLOAT, // тип координат вершин в частности и тип данных аттрибута в общем
            GL_FALSE, // нужно ли нормализовать?
            0, // шаг между атрибутами вершин 
            nullptr // смещение того места где начинаются данные в буффере
        );
        glEnableVertexAttribArray(1);

        glDrawArrays(GL_TRIANGLES, 0, 12*3); // начиная с вершины 0 нарисовать все 3 вершины которые образуют треугольник
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Меняем буффер
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0); // Проверяем Escape на нажатие, или пользователь вручную закрыл окно
}