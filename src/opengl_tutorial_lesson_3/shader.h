#ifndef SHADER_H
#define SHADER_H

#include <GL\glew.h>

GLuint loadShaders(const char *vertexFilePath, const char *fragmentFilePath);

#endif // SHADER_H