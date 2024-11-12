#include "OGLProgram.h"
#include <glad/glad.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <stdexcept>

static std::string ReadFile(const std::string& InPath) 
{
    std::ifstream InputStream(InPath, std::ios::ate);
    size_t FileSize = InputStream.tellg(); //cursor position in bytes.

    std::string Text;
    Text.resize(FileSize);

    InputStream.seekg(0, std::ios::beg); //rewind cursor position
    InputStream.read(&Text[0], FileSize); // read the whole file

    InputStream.close();
    return Text;
}

static GLuint CreateShader(const std::string& InPath, GLuint InShaderType) 
{
    std::string Text = ReadFile(InPath);
    const char* ShaderSource = Text.c_str();
    
    GLuint ShaderId = glCreateShader(InShaderType);
    glShaderSource(ShaderId, 1, &ShaderSource, NULL);
    glCompileShader(ShaderId);

    GLint Success;
    glGetShaderiv(ShaderId, GL_COMPILE_STATUS, &Success);
    if(!Success) //0 = false, 1 = true
    {
        GLint MaxLogLength;
        glGetShaderiv(ShaderId, GL_INFO_LOG_LENGTH, &MaxLogLength);
        
        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetShaderInfoLog(ShaderId, MaxLogLength, NULL, InfoLog.data());

        std::string LogStr(InfoLog.begin(), InfoLog.end());
        std::cout << "Shader Compilation failure: " << LogStr;
        throw std::runtime_error(LogStr);
    }

    return ShaderId;
}

static GLuint CreateProgram(GLuint VertexId, GLuint FragmentId) 
{
    GLuint ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexId);
    glAttachShader(ProgramId, FragmentId);
    glLinkProgram(ProgramId);

    GLint Success;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &Success);
    if(!Success) //0 = false, 1 = true
    {
        GLint MaxLogLength;
        glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &MaxLogLength);
        
        std::vector<GLchar> InfoLog(MaxLogLength);
        glGetProgramInfoLog(ProgramId, MaxLogLength, NULL, InfoLog.data());

        std::string LogStr(InfoLog.begin(), InfoLog.end());
        std::cout << "Program Linking failure: " << LogStr;
        throw std::runtime_error(LogStr);
    }

    glDeleteShader(VertexId);
    glDeleteShader(FragmentId);
    return ProgramId;
}


OGLProgram::OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath)
{
    //CreateShader vert  (Read + Compile)
    GLuint VertexShaderId = CreateShader(InVertShaderPath, GL_VERTEX_SHADER);
    GLuint FragmentShaderId = CreateShader(InFragShaderPath, GL_FRAGMENT_SHADER);

    //Create program (Linking)
    ProgramId = CreateProgram(VertexShaderId, FragmentShaderId);
}

OGLProgram::~OGLProgram() 
{
    glDeleteProgram(ProgramId);
}

void OGLProgram::Bind() 
{
    glUseProgram(ProgramId);
}

GLuint OGLProgram::GetId() 
{
    return ProgramId;
}

void OGLProgram::SetUniform(const std::string& InName, const Color& InColor)
{
    glUniform4fv(glGetUniformLocation(GetId(), InName.c_str()), 1, reinterpret_cast<const GLfloat*>(&InColor));
}

void OGLProgram::SetUniform(const std::string& InName, float InValue)
{
    glUniform1f(glGetUniformLocation(GetId(), InName.c_str()), InValue);
}

void OGLProgram::SetUniform(const std::string& InName, const glm::mat4& InMatrix)
{
    glUniformMatrix4fv(glGetUniformLocation(GetId(), InName.c_str()), 1, GL_FALSE, &InMatrix[0][0]);
}

void OGLProgram::SetUniform(const std::string& InName, const glm::vec3& InVec3)
{
    glUniform3fv(glGetUniformLocation(GetId(), InName.c_str()), 1, &InVec3[0]);
}