#pragma once
#include <string>
#include <glad/glad.h>
#include "Common.h"
#include <glm/glm.hpp>

class OGLProgram 
{
public:
    OGLProgram(const std::string& InVertShaderPath, const std::string& InFragShaderPath);
    ~OGLProgram();
    void Bind();
    GLuint GetId();
    void SetUniform(const std::string& InName, const Color& InColor);
    void SetUniform(const std::string& InName, float InValue);
    void SetUniform(const std::string& InName, const glm::mat4& InMatrix);
    void SetUniform(const std::string& InName, const glm::vec3& InVec3);
private:
    GLuint ProgramId;
};