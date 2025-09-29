#ifndef VRAKTAL_CORE_RHI_COMMAND_BUFFER_VK_H
#define VRAKTAL_CORE_RHI_COMMAND_BUFFER_VK_H
#pragma once

#include <core/rhi/commandBuffer.h>
#include <vulkan/vulkan.h>

namespace core::rhi
{
    namespace vulkan { class GpuDeviceVulkan; class ImageVulkan; class PipelineVulkan; } // TODO : Remove vulkan namespace once refacto has been done.

    struct CommandBuffer::Impl
    {
    public:
        Impl(vulkan::GpuDeviceVulkan& device); // TODO : Remove vulkan namespace once refacto has been done.
        ~Impl();

        void Begin();
        void End();

        void BeginRendering(const RenderingInfo& info, uint32_t imageIndex);
        void EndRendering(uint32_t imageIndex);

        void BindPipeline(Pipeline* pipeline);
        void Draw(uint32_t vertexCount, uint32_t width, uint32_t height);

        void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

        VkCommandBuffer GetNative() const { return m_commandBuffer; }

    private:
        vulkan::GpuDeviceVulkan& m_device;
        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        VkCommandPool m_pool = VK_NULL_HANDLE;
    };
}

#endif //VRAKTAL_CORE_RHI_COMMAND_BUFFER_VK_H
