#include "Ex11PostFX.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "obj_parser.h"


void Ex11PostFX::Start() {

    SceneProgram = new OGLProgram("resources/shaders/postfx_scene.vert", "resources/shaders/postfx_scene.frag");

    //Counter-Clock wise. vertex data are no more in NDC space (because of the Z)
    std::vector<float> Vertices = {
        //positions      uvs
         //FRONT FACE
        -1, -1, 1,   0, 0,    //bottom-left
         1, -1, 1,   1, 0,    //bottom-right
         1,  1, 1,   1, 1,    //top-right
        -1,  1, 1,   0, 1,    //top-left 
        -1, -1, 1,   0, 0,    //bottom-left
         1,  1, 1,   1, 1,    //top-right

        //BACK FACE
         1, -1,-1,   0, 0,    //bottom-left
        -1, -1,-1,   1, 0,    //bottom-right
        -1,  1,-1,   1, 1,    //top-right
         1,  1,-1,   0, 1,    //top-left 
         1, -1,-1,   0, 0,    //bottom-left
        -1,  1,-1,   1, 1,    //top-right

         //LEFT FACE
        -1, -1,-1,   0, 0,    //bottom-left
        -1, -1, 1,   1, 0,    //bottom-right
        -1,  1, 1,   1, 1,    //top-right
        -1,  1,-1,   0, 1,    //top-left 
        -1, -1,-1,   0, 0,    //bottom-left
        -1,  1, 1,   1, 1,    //top-right

        //RIGHT FACE
         1, -1, 1,   0, 0,    //bottom-left
         1, -1,-1,   1, 0,    //bottom-right
         1,  1,-1,   1, 1,    //top-right
         1,  1, 1,   0, 1,    //top-left 
         1, -1, 1,   0, 0,    //bottom-left
         1,  1,-1,   1, 1,    //top-right

         //TOP FACE
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1, 1,   1, 0,    //bottom-right
          1, 1,-1,   1, 1,    //top-right
         -1, 1,-1,   0, 1,    //top-left 
         -1, 1, 1,   0, 0,    //bottom-left
          1, 1,-1,   1, 1,    //top-right
         
         //BOTTOM FACE
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1,-1,   1, 0,    //bottom-right
          1,-1, 1,   1, 1,    //top-right
         -1,-1, 1,   0, 1,    //top-left 
         -1,-1,-1,   0, 0,    //bottom-left
          1,-1, 1,   1, 1,    //top-right
    };

    //1. Create VAO
    glGenVertexArrays(1, &SceneVao);
    glBindVertexArray(SceneVao);

    //2. Create VBO to load data
    glGenBuffers(1, &SceneVbo);
    glBindBuffer(GL_ARRAY_BUFFER, SceneVbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);

    //5. Create Texture
    BoxTexture = new OGLTexture("resources/textures/wood-box.jpg");
    
    //Camera
    glm::vec3 Position = glm::vec3(0, 0, 5);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01f;
    float ZFar = 100.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);

    // PostFX: Prepare Framebuffer for drawing "off-screen"
    glGenFramebuffers(1, &SceneFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);

    //1. Attach Color to framebuffer
    glGenTextures(1, &SceneTexture);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, SceneTexture, 0);

    //2. Attach Depth to framebuffer
    glGenRenderbuffers(1, &SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, SceneRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, SceneRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind current render buffer

    bool rboSuccess = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!rboSuccess) {
        DIE("Framebuffer not ready!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind and back to Swapchain framebuffer

    // Define PostFX pipeline
    FxProgram = new OGLProgram("resources/shaders/postfx_effect.vert", "resources/shaders/postfx_effect.frag");

    std::vector<float> QuadVertices = {
        // Positions   // Uvs
        -1.f, -1.f,  0.f, 0.f,  //bottom left
         1.f, -1.f,  1.f, 0.f,  //bottom right
        -1.f,  1.f,  0.f, 1.f,  //top left

        -1.f,  1.f,  0.f, 1.f,  //top left
         1.f, -1.f,  1.f, 0.f,  //bottom right
         1.f,  1.f,  1.f, 1.f   //top right  
    };


    //1. Create VAO
    glGenVertexArrays(1, &FxVao);
    glBindVertexArray(FxVao);

    //2. Create VBO to load data
    glGenBuffers(1, &FxVbo);
    glBindBuffer(GL_ARRAY_BUFFER, FxVbo);

    glBufferData(GL_ARRAY_BUFFER, QuadVertices.size() * sizeof(float), QuadVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

}

void Ex11PostFX::Update(float InDeltaTime) {

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    
    
    //1. Draw Scene (Pass 1)
    glBindFramebuffer(GL_FRAMEBUFFER, SceneFbo);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, 0, -1));
    Model = glm::scale(Model, glm::vec3(1, 1, 1));
    glm::mat4 Mvp = Projection * View * Model;
    
    glBindVertexArray(SceneVao);
    SceneProgram->Bind();
    SceneProgram->SetUniform("mvp", Mvp);
    BoxTexture->Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    //2. Post FX (Pass 2)
    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind and back to Swapchain framebuffer
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(FxVao);
    FxProgram->Bind();
    FxProgram->SetUniform("time", ElapsedTime);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, SceneTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Ex11PostFX::Destroy() {
    glDeleteVertexArrays(1, &SceneVao);
    glDeleteBuffers(1, &SceneVbo);
    delete SceneProgram;

    glDeleteFramebuffers(1, &SceneFbo);
    glDeleteTextures(1, &SceneTexture);
    glDeleteRenderbuffers(1, &SceneRbo);
    glDeleteVertexArrays(1, &FxVao);
    glDeleteBuffers(1, &FxVbo);
    delete FxProgram;
}

