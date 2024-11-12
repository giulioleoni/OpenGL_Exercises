#include "Ex01TriangleDraw.h"
#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>

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

void Ex01TriangleDraw::Start() {
    //ReadFile("resources/shaders/triangle.vert");
    //ReadFile("resources/shaders/triangle.frag");

    //CreateShader vert  (Read + Compile)
    GLuint VertexShaderId = CreateShader("resources/shaders/triangle.vert", GL_VERTEX_SHADER);
    GLuint FragmentShaderId = CreateShader("resources/shaders/triangle.frag", GL_FRAGMENT_SHADER);

    //Create program (Linking)
    ProgramId = CreateProgram(VertexShaderId, FragmentShaderId);


    //Counter-Clock wise. By now vertex data in NDC space
    std::vector<float> Vertices = {
         0.5f, -0.5f, 0.f,  //bottom-right
        -0.5f, -0.5f, 0.f,  //bottom-left
         0.0f,  0.5f, 0.f,  //top
    };


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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    //4. Set Viewport
    glViewport(0, 0, 800, 600);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glUseProgram(ProgramId);
}

void Ex01TriangleDraw::Update(float InDeltaTime) {
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Ex01TriangleDraw::Destroy() {
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    glDeleteProgram(ProgramId);
}

