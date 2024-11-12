#pragma once
#include <string>

typedef struct GLFWwindow GLFWwindow;

class Window
{
public:
    Window(int InWidth, int InHeight, const std::string& InTitle);
    bool IsOpened();
    float DeltaTime();
    void SetTitle(const std::string& InTitle);
    void Update();
    ~Window();

private:
    GLFWwindow* RawWindow;
    float m_DeltaTime;
};