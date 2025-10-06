#include "../src/core/rhi/commandBuffer_impl_vulkan.h"
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"
#include "../src/core/rhi/pipeline_impl_vulkan.h"
#include "../src/core/rhi/gpuImage_impl_vulkan.h"
#include "../gpu_details/initializer_vulkan.h"

#include <core/rhi/renderingInfo.h>
#include <stdexcept>

using namespace core::rhi;


CommandBuffer::CommandBuffer(GpuDevice& device)
    : m_impl(std::make_unique<Impl>(device))
{
}

CommandBuffer::~CommandBuffer() = default;

void CommandBuffer::Begin() { m_impl->Begin(); }
void CommandBuffer::End() { m_impl->End(); }
void CommandBuffer::BeginRendering(const RenderingInfo& info, uint32_t imageIndex) { m_impl->BeginRendering(info, imageIndex); }
void CommandBuffer::EndRendering(uint32_t imageIndex) { m_impl->EndRendering(imageIndex); }
void CommandBuffer::BindPipeline(Pipeline* pipeline) { m_impl->BindPipeline(pipeline); }
void CommandBuffer::Draw(uint32_t vertexCount, uint32_t width, uint32_t height) { m_impl->Draw(vertexCount, width, height); }

CommandBuffer::Impl& CommandBuffer::GetImpl() { return *m_impl; }

CommandBuffer::Impl::Impl(GpuDevice& _device)
    : m_device(_device)
{
    VkCommandPoolCreateInfo poolInfo = gpu_details::CommandPoolCreateInfo(m_device.GetImpl().m_graphicsQueueFamily);

    if (vkCreateCommandPool(m_device.GetImpl().m_device, &poolInfo, nullptr, &m_pool) != VK_SUCCESS)
        throw std::runtime_error("Failed to create command pool");

    VkCommandBufferAllocateInfo allocInfo = gpu_details::CommandBufferAllocateInfo(m_pool);

    if (vkAllocateCommandBuffers(m_device.GetImpl().m_device, &allocInfo, &m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate command buffer");
    }
}

CommandBuffer::Impl::~Impl()
{
    if (m_commandBuffer != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(m_device.GetImpl().m_device, m_pool, 1, &m_commandBuffer);
    }
    if (m_pool != VK_NULL_HANDLE)
    {
        vkDestroyCommandPool(m_device.GetImpl().m_device, m_pool, nullptr);
    }
}

void CommandBuffer::Impl::Begin()
{
    vkResetCommandBuffer(m_commandBuffer, 0);

    VkCommandBufferBeginInfo beginInfo = gpu_details::CommandBufferBeginInfo()

    if (vkBeginCommandBuffer(m_commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to begin recording command buffer");
    }
}

void CommandBuffer::Impl::End()
{
    if (vkEndCommandBuffer(m_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("vkEndCommandBuffer failed");
    }
}

void CommandBuffer::Impl::BeginRendering(const RenderingInfo& info, uint32_t imageIndex)
{
    std::vector<VkRenderingAttachmentInfo> attachments;
    auto* swapImg = static_cast<GpuImage*>(m_device.GetImpl().GetSwapchainImage(imageIndex));

    for (auto& att : info.colorAttachments)
    {
        VkRenderingAttachmentInfo vkAttachmentInfo = gpu_details::CreateRenderingAttachmentInfo(swapImg->GetImpl().GetView(),
            att.loadOp, att.storeOp, { att.clearValue.r, att.clearValue.g, att.clearValue.b, att.clearValue.a });
        attachments.push_back(vkAttachmentInfo);
    }

    VkRenderingAttachmentInfo depthAttachment = gpu_details::CreateDepthAttachmentInfo(m_device.GetImpl().m_depthImageView);
    VkRenderingInfo vkInfo = gpu_details::CreateRenderingInfo(info.width, info.height, attachments, &depthAttachment);

    TransitionImageLayout(
        swapImg->GetImpl().GetNative(),
        swapImg->GetImpl().Format(),
        swapImg->GetImpl().CurrentLayout(),
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
    );

    vkCmdBeginRendering(m_commandBuffer, &vkInfo);
}

void CommandBuffer::Impl::EndRendering(uint32_t imageIndex)
{
    auto* swapImg = static_cast<GpuImage*>(m_device.GetImpl().GetSwapchainImage(imageIndex));

    vkCmdEndRendering(m_commandBuffer);

    TransitionImageLayout(
        swapImg->GetImpl().GetNative(),
        swapImg->GetImpl().Format(),
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    );

    swapImg->GetImpl().SetLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
}

void CommandBuffer::Impl::BindPipeline(Pipeline* pipeline)
{
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetImpl().GetNative());
}

void CommandBuffer::Impl::Draw(uint32_t vertexCount, uint32_t width, uint32_t height)
{
    VkViewport viewport = gpu_details::CreateViewport(width, height);
    vkCmdSetViewport(m_commandBuffer, 0, 1, &viewport);

    VkRect2D scissor = gpu_details::CreateScissor(width, height);
    vkCmdSetScissor(m_commandBuffer, 0, 1, &scissor);

    vkCmdDraw(m_commandBuffer, vertexCount, 1, 0, 0);
}

void CommandBuffer::Impl::TransitionImageLayout(VkImage image, VkFormat, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    auto barrierInfo = gpu_details::CreateImageMemoryBarrier(
        image,
        oldLayout,
        newLayout
    );

    vkCmdPipelineBarrier(
        m_commandBuffer,
        barrierInfo.srcStage,
        barrierInfo.dstStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrierInfo.barrier
    );
}