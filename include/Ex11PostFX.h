#pragma once
#include "glad/glad.h"
#include "OGLProgram.h"
#include "OGLTexture.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
class Ex11PostFX 
{
public:
    void Start();
    void Update(float InDeltaTime);
    void Destroy();

private:
    GLuint SceneVao;
    GLuint SceneVbo;
    OGLProgram* SceneProgram;
    OGLTexture* BoxTexture;
    glm::mat4 View;
    glm::mat4 Projection;
    size_t VerticeCount;

    GLuint SceneFbo;
    GLuint SceneTexture;
    GLuint SceneRbo;
    OGLProgram* FxProgram;
    GLuint FxVao;
    GLuint FxVbo;
};