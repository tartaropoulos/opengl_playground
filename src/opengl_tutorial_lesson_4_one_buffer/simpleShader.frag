#version 460 core

// in vec3 vert;
in vec3 fragmentColor;

out vec3 color;
void main()
{
    color = fragmentColor;
}