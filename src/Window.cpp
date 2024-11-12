#include "Window.h"
//NOTE: Remeber to include GLAD before GLFW3
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

Window::Window(int InWidth, int InHeight, const std::string& InTitle)
{
    if (glfwInit() == 0) 
	{
        throw std::runtime_error("Error glfw init");
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	GLFWmonitor* monitor = NULL;

	RawWindow = glfwCreateWindow(InWidth, InHeight, InTitle.c_str(), monitor, NULL);
	if (RawWindow == NULL) 
    {
        throw std::runtime_error("Error creating window");
	}

	glfwMakeContextCurrent(RawWindow);

	if (gladLoadGL() == 0) 
	{
        throw std::runtime_error("Error loading GL Functions");
	}

    m_DeltaTime = 0;
}

Window::~Window()
{
    glfwDestroyWindow(RawWindow);
}


bool Window::IsOpened()
{
    return !glfwWindowShouldClose(RawWindow);
}

float Window::DeltaTime()
{
    return m_DeltaTime;
}

void Window::SetTitle(const std::string& InTitle)
{
    glfwSetWindowTitle(RawWindow, InTitle.c_str());
}

void Window::Update()
{
    static float LastTime = glfwGetTime();

    float CurrTime = glfwGetTime();
	m_DeltaTime = CurrTime - LastTime;
    LastTime = CurrTime;

    glfwSwapBuffers(RawWindow);
	glfwPollEvents();
}