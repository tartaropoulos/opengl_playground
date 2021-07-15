#version 460 core

layout(location = 0) in vec3 vertexPositionModelSpace;

// out vec3 vert;

// Значение остается константой для всех вершин в меше
uniform mat4 MVP;

void main()
{
    // vert = vertexPositionModelSpace;
    gl_Position = MVP * vec4(vertexPositionModelSpace, 1);
}