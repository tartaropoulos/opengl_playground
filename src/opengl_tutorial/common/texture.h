#ifndef TEXTURE_H
#define TEXTURE_H

#include <filesystem>

#include <GL/glew.h>

GLuint loadBMP_custom(const std::filesystem::path texturePath);

GLuint loadDDS(std::filesystem::path texturePath);

#endif // TEXTURE_H