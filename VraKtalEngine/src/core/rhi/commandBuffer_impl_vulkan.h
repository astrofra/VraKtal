#ifndef VRAKTAL_CORE_RHI_COMMAND_BUFFER_VK_H
#define VRAKTAL_CORE_RHI_COMMAND_BUFFER_VK_H
#pragma once

#include <core/rhi/commandBuffer.h>
#include <vulkan/vulkan.h>

namespace core::rhi::vulkan { class GpuDeviceVulkan; } // TODO : Remove once refacto finished

namespace core::rhi
{
    struct CommandBuffer::Impl
    {
        vulkan::GpuDeviceVulkan& device; // TODO : Remove namespace once refacto finished
        VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
        VkCommandPool pool;

        Impl(vulkan::GpuDeviceVulkan& _device);
        ~Impl();

        void Begin();
        void End();

        void BeginRendering(const RenderingInfo& info, uint32_t imageIndex);
        void EndRendering(uint32_t imageIndex);

        void BindPipeline(Pipeline* pipeline);
        void Draw(uint32_t vertexCount, uint32_t width, uint32_t height);

        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        VkCommandBuffer GetNative() const { return m_commandBuffer; }
    };
}

#endif //VRAKTAL_CORE_RHI_COMMAND_BUFFER_VK_H
