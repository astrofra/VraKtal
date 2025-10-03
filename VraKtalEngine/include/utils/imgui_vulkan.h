#ifndef VRAKTAL_UTILS_IMGUI_VK_H
#define VRAKTAL_UTILS_IMGUI_VK_H

#include <GLFW/glfw3.h>
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"

namespace utils
{
	void InitImGui(core::rhi::GpuDevice& device, GLFWwindow* window);
}

#endif //VRAKTAL_CORE_UTILS_IMGUI_VK_H