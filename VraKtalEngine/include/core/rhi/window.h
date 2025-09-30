
#ifndef VRAKTAL_CORE_RHI_WINDOW_H
#define VRAKTAL_CORE_RHI_WINDOW_H
#pragma once

#include <cstdint>
#include <utility>
#include <string>


struct GLFWwindow;

namespace core::rhi
{
    class Window {

    public:
        Window(uint32_t _width, uint32_t _height, const char* _title, bool _resizable = true);
        ~Window();

        void PollEvents();
        bool ShouldClose() const;

        std::pair<uint32_t, uint32_t> Size() const;
        const char* Title() const;

        GLFWwindow* m_window;
        uint32_t m_width = 0, m_height = 0;
        std::string m_title;

        GLFWwindow* GlfwHandle() const;
    };
}

#endif //VRAKTAL_CORE_RHI_WINDOW_H