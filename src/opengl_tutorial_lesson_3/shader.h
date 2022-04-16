#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC
#include <GL\glew.h>

GLuint loadShaders(const char *vertexFilePath, const char *fragmentFilePath);

#endif // SHADER_H