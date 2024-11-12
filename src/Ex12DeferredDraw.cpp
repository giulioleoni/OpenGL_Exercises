#include "Ex12DeferredDraw.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "obj_parser.h"


void Ex12DeferredDraw::Start() {

    #pragma region Scene

    Program = new OGLProgram("resources/shaders/deferred_scene.vert", "resources/shaders/deferred_scene.frag");

    obj_t* mesh = obj_parse("resources/models/stormtrooper.obj");
    
    std::vector<float> Vertices;       
    for(int i=0; i < mesh->triangle_count; ++i) {
        obj_triangle_t& t = mesh->triangles[i];

        Vertices.push_back(t.v1.position.x);
        Vertices.push_back(t.v1.position.y);
        Vertices.push_back(t.v1.position.z);
        Vertices.push_back(t.v1.uv.x);
        Vertices.push_back(t.v1.uv.y);
        Vertices.push_back(t.v1.normal.x);
        Vertices.push_back(t.v1.normal.y);
        Vertices.push_back(t.v1.normal.z);

        Vertices.push_back(t.v2.position.x);
        Vertices.push_back(t.v2.position.y);
        Vertices.push_back(t.v2.position.z);
        Vertices.push_back(t.v2.uv.x);
        Vertices.push_back(t.v2.uv.y);
        Vertices.push_back(t.v2.normal.x);
        Vertices.push_back(t.v2.normal.y);
        Vertices.push_back(t.v2.normal.z);

        Vertices.push_back(t.v3.position.x);
        Vertices.push_back(t.v3.position.y);
        Vertices.push_back(t.v3.position.z);
        Vertices.push_back(t.v3.uv.x);
        Vertices.push_back(t.v3.uv.y);
        Vertices.push_back(t.v3.normal.x);
        Vertices.push_back(t.v3.normal.y);
        Vertices.push_back(t.v3.normal.z);
    }
    VerticeCount = mesh->triangle_count * 3;
    obj_parse_destroy(mesh);

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind(); 

    //5. Create Texture
    StormTexture = new OGLTexture("resources/models/stormtrooper.png");

    //Camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60.f;
    float AspectRatio = 800.f / 600.f;
    float ZNear = 0.01f;
    float ZFar = 100.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);
    #pragma endregion

    //Creating Framebuffer as G-Buffer (alias Geometry Buffer) (MRT - Multi Render Target)

    #pragma region GBuffer
    
    glGenFramebuffers(1, &GFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, GFbo);

    //1. Render Target Diffuse (3xFloat => 8 bit precision)
    glGenTextures(1, &DiffuseTexture);
    glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, DiffuseTexture, 0);

    //2. Render Target Normal (3xFloat => 16 bit precision)
    glGenTextures(1, &NormalTexture);
    glBindTexture(GL_TEXTURE_2D, NormalTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, NormalTexture, 0);

    //3. Render Target Posittion (3xFloat => 16 bit precision)
    glGenTextures(1, &PositionTexture);
    glBindTexture(GL_TEXTURE_2D, PositionTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, PositionTexture, 0);

    GLenum Attachs[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, Attachs);


    //4. Attach Depth to framebuffer
    glGenRenderbuffers(1, &DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, DepthRbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind current render buffer

    bool rboSuccess = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    if (!rboSuccess) {
        DIE("Framebuffer not ready!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    #pragma endregion

    BlendingProgram = new OGLProgram("resources/shaders/deferred_blend.vert", "resources/shaders/deferred_blend.frag");
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
    glGenVertexArrays(1, &BlendVao);
    glBindVertexArray(BlendVao);

    //2. Create VBO to load data
    glGenBuffers(1, &BlendVbo);
    glBindBuffer(GL_ARRAY_BUFFER, BlendVbo);

    glBufferData(GL_ARRAY_BUFFER, QuadVertices.size() * sizeof(float), QuadVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    BlendingProgram->Bind();
    BlendingProgram->SetUniform("camera_pos", Position);

    //Light
    glm::vec3 PointLightRightPos = glm::vec3(4, 0, 0);
    glm::vec3 PointLightLeftPos = glm::vec3(-4, 0, 0);
    glm::vec3 PointLightUpPos = glm::vec3(0, 4, 0);
    BlendingProgram->SetUniform("point_light_poses[0]", PointLightRightPos);
    BlendingProgram->SetUniform("point_light_poses[1]", PointLightLeftPos);
    BlendingProgram->SetUniform("point_light_poses[2]", PointLightUpPos);
}

static void DebugGBuffer(GLuint InFbo) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);  //GL_FRAMEBUFFER = Read + Write operation
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, InFbo); //GL_READ_FRAMEBUFFER = Read operation
    
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(
        0,   0, 800, 600,  //Source Bounds
        0, 300, 400, 600,  //Destin Bounds
        GL_COLOR_BUFFER_BIT,  //Which buffer to write to
        GL_LINEAR             //interpolation in case of streching image (LINEAR or NEAREST)
    );

    glReadBuffer(GL_COLOR_ATTACHMENT1);
    glBlitFramebuffer(
          0,   0, 800, 600,  //Source Bounds
        400, 300, 800, 600,  //Destin Bounds
        GL_COLOR_BUFFER_BIT,  //Which buffer to write to
        GL_LINEAR             //interpolation in case of streching image (LINEAR or NEAREST)
    );

    glReadBuffer(GL_COLOR_ATTACHMENT2);
    glBlitFramebuffer(
          0,   0, 800, 600,  //Source Bounds
          0,   0, 400, 300,  //Destin Bounds
        GL_COLOR_BUFFER_BIT,  //Which buffer to write to
        GL_LINEAR             //interpolation in case of streching image (LINEAR or NEAREST)
    );
}

void Ex12DeferredDraw::Update(float InDeltaTime) {
    
    //Geometry Pass
    glBindFramebuffer(GL_FRAMEBUFFER, GFbo);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    float Angle = 20.f * ElapsedTime;

    //Model = Translate * Rotate * Scale
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;
    Program->Bind();
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);

    //Bind Stormtrooper mesh
    glBindVertexArray(Vao);
    StormTexture->Bind(GL_TEXTURE0);

    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);

    //DebugGBuffer(GFbo);
    //2. Post FX (Pass 2)

    glBindFramebuffer(GL_FRAMEBUFFER, 0); //Unbind and back to Swapchain framebuffer
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT);

    BlendingProgram->Bind();
    glBindVertexArray(BlendVao);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, DiffuseTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, NormalTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, PositionTexture);

    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Ex12DeferredDraw::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete StormTexture;
    delete Program;


    glDeleteFramebuffers(1, &GFbo);
    glDeleteTextures(1, &DiffuseTexture);
    glDeleteTextures(1, &NormalTexture);
    glDeleteTextures(1, &PositionTexture);
    glDeleteRenderbuffers(1, &DepthRbo);

    glDeleteVertexArrays(1, &BlendVao);
    glDeleteBuffers(1, &BlendVbo);

    delete BlendingProgram;
}

