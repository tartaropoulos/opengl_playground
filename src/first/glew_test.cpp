#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <GL\glut.h>
#include <glm\glm.hpp>

int main(int argc, char **argv)
{
    glewInit();
    glutInit(&argc, argv);
    glfwInit();
    glm::mat2 identity{0, 1, 0, 1};
    std::cout << "Hello OpenGL!" << std::endl;
    std::cout << identity[0][0] << identity[0][1] << identity[1][0] << identity[1][1] << std::endl;

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	glutCreateWindow("Lesson 1");

    system("pause");
    return 0;
}