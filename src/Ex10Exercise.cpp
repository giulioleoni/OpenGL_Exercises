#include "Ex10Exercise.h"
#include <string>
#include <vector>
#include "OGLProgram.h"
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "obj_parser.h"


void Ex10Exercise::Start() {

    #pragma region StormTrooper
    Program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/phong.frag");

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
    
    //6. Enable Dept Testing
    glEnable(GL_DEPTH_TEST);

    //7. Enable Cull Face
    glEnable(GL_CULL_FACE);

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

    Program->SetUniform("camera_pos", Position);


    #pragma endregion

    ProgramLight = new OGLProgram("resources/shaders/light.vert", "resources/shaders/light.frag");
    std::vector<float> LightVertices = {
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
    glGenVertexArrays(1, &VaoLight);
    glBindVertexArray(VaoLight);

    //2. Create VBO to load data
    glGenBuffers(1, &VboLight);
    glBindBuffer(GL_ARRAY_BUFFER, VboLight);

    int LightDataSize = LightVertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, LightDataSize, LightVertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint LightLocation_0 = 0;
    glVertexAttribPointer(LightLocation_0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(LightLocation_0);

    GLuint LightLocation_1 = 1;
    glVertexAttribPointer(LightLocation_1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(LightLocation_1);

    //5. Create Texture
    LightTexture = new OGLTexture("resources/textures/wood-box.jpg");


}

void Ex10Exercise::Update(float InDeltaTime) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    float Angle = 20.f * ElapsedTime;


    //Light
    ProgramLight->Bind();

    static glm::vec3 LightStartingPosition = glm::vec3(4, 0, 0);

    glBindVertexArray(VaoLight);

    glm::mat4 RotateAround = glm::mat4(1.f);

    RotateAround = glm::translate(RotateAround, StormPosition);
    RotateAround = glm::rotate(RotateAround, glm::radians(Angle), glm::vec3(0, 1, 0));
    RotateAround = glm::translate(RotateAround, -StormPosition);

    glm::mat4 LightModel = glm::mat4(1.f);

    LightModel = glm::translate(LightModel, LightStartingPosition);
    LightModel = glm::scale(LightModel, glm::vec3(.25f));
    

    glm::mat4 MvpLight = Projection * View * RotateAround * LightModel;
    LightTexture->Bind(GL_TEXTURE0);
    ProgramLight->SetUniform("mvp", MvpLight);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    //Stormtrooper
    StormPosition = glm::vec3(0, -4, 0);
    Program->Bind();
    glBindVertexArray(Vao);
    
    //Model = Translate * Rotate * Scale
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, StormPosition);
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;
    //Light

    glm::mat4 LightWorldPositionMatrix = RotateAround * LightModel;
    glm::vec4 LightWorldPos = LightWorldPositionMatrix * glm::vec4(LightStartingPosition, 1.f);

    glm::vec3 PointLightPos = glm::vec3(LightWorldPos);
    Program->SetUniform("point_light_pos", PointLightPos);
    Program->SetUniform("mvp", Mvp);
    Program->SetUniform("model", Model);

    StormTexture->Bind(GL_TEXTURE0);
    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);
}

void Ex10Exercise::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteVertexArrays(1, &VaoLight);
    glDeleteBuffers(1, &VboLight);
    delete StormTexture;
    delete LightTexture;
    delete Program;
    delete ProgramLight;
}

