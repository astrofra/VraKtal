#include <core/window.h>

#include <GLFW/glfw3.h>
#pragma comment(lib, "glfw3.lib")

#include <stdexcept>

using namespace core;

Window::Window(uint32_t _width, uint32_t _height, const char* _title, bool _resizable)
    : m_title(_title), m_width(_width), m_height(_height)
{
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, _resizable ? GLFW_TRUE : GLFW_FALSE);

    m_window = glfwCreateWindow((int)m_width, (int)m_height, m_title.c_str(), nullptr, nullptr);

    if (!m_window)
    {
        throw std::runtime_error("Failed to create GLFW window");
    }
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }
}

void Window::PollEvents()
{
    glfwPollEvents();
}

bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

std::pair<uint32_t, uint32_t> Window::Size() const
{
    return { m_width, m_height };
}

const char* core::Window::Title() const
{
    return m_title.c_str();
}

GLFWwindow* core::Window::GlfwHandle() const 
{
    return m_window;
}
