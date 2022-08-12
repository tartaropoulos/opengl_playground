#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "shader.h"

std::string readShaderCode(const char *filePath)
{
    std::string shaderCode;
    std::ifstream shaderStream(filePath, std::ios::in);

    if(shaderStream.is_open())
    {
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        shaderCode = sstr.str();
        shaderStream.close();
    }
    else
    {
        std::cout << "Can't open " << filePath << std::endl;
        getchar();
        return std::string("");
    }

    return shaderCode;
}

bool hasCompiled(int logLength, GLuint ID)
{
    if(logLength > 0)
    {
        std::vector<char> shaderErrorMessage(++logLength);
        glGetShaderInfoLog(ID, logLength, NULL, &shaderErrorMessage[0]);
        std::cout << &shaderErrorMessage[0] << std::endl;

        return false;
    }

    return true;
}

bool compileShader(std::string shaderCode, GLuint shaderID, const char *filePath)
{
    std::cout << "Compile shader: " << filePath << std::endl;
    char const *shaderSource = shaderCode.c_str();

    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    GLint result = GL_FALSE;
    int logLength;

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

    return hasCompiled(logLength, shaderID);
}

GLuint loadShaders(const char *vertexFilePath, const char *fragmentFilePath)
{
    GLuint vertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertexShaderCode   = readShaderCode(vertexFilePath);
    std::string fragmentShaderCode = readShaderCode(fragmentFilePath);

    bool hasVertexShaderCompiled = compileShader(vertexShaderCode, vertexShaderID, vertexFilePath);
    bool hasFragmentShaderCompiled = compileShader(fragmentShaderCode, fragmentShaderID, fragmentFilePath);

    GLuint programID = glCreateProgram();

    if(hasVertexShaderCompiled && hasFragmentShaderCompiled)
    {
        std::cout << "Linking program\n";
        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        GLint result = GL_FALSE;
        int logLength;

        glGetProgramiv(programID, GL_COMPILE_STATUS, &result);
        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

        bool hasProgramCompiled = hasCompiled(logLength, programID);

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);

        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        if(!hasProgramCompiled)
        {
            std::cout << "Program compile is failed\n";
            // return 0;
        }
    }
    else
    {
        std::cout << "Shaders compile is failed\n";
        // return 0;
    }  

    return programID;
}
