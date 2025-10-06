#ifndef VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
#define VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include <core/rhi/buffer.h>
#include <core/rhi/renderingInfo.h>
#include <core/rhi/gpuDevice.h>

#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"

#include "converter_vulkan.h"

namespace core::gpu_details
{
    VkBufferCreateInfo              BufferCreateInfo(size_t size, core::rhi::BufferUsage usage);
    VkMemoryAllocateInfo            BufferCreateAllocateInfo(uint64_t allocationSize, uint32_t memoryTypeIndex);

    VkDescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo(std::vector<VkDescriptorSetLayoutBinding> bindings);
    VkDescriptorSetAllocateInfo     DescriptorSetCreateAllocateInfo(VkDescriptorSetLayout layout, VkDescriptorPool pool);
    VkDescriptorBufferInfo          DescriptorSetCreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);
    VkWriteDescriptorSet            DescriptorSetCreateWriteBuffer(VkDescriptorSet set, uint32_t binding, const VkDescriptorBufferInfo* bufferInfo);
    VkDescriptorImageInfo           DescriptorSetCreateImageInfo(VkImageView imageView, VkImageLayout layout, VkSampler sampler);
    VkWriteDescriptorSet            DescriptorSetCreateWriteImage(VkDescriptorSet set, uint32_t binding, const VkDescriptorImageInfo* imageInfo);

    VkCommandPoolCreateInfo         CommandPoolCreateInfo(uint32_t graphicsQueueFamily);
    VkCommandBufferAllocateInfo     CommandBufferAllocateInfo(VkCommandPool pool);
    VkCommandBufferBeginInfo        CommandBufferBeginInfo();

    VkRenderingAttachmentInfo       CreateRenderingAttachmentInfo(VkImageView imageView, core::rhi::LoadOp loadOp, core::rhi::StoreOp storeOp, const VkClearColorValue& clearValue);
    VkRenderingAttachmentInfo       CreateDepthAttachmentInfo(VkImageView depthView);
    VkRenderingInfo                 CreateRenderingInfo(uint32_t width, uint32_t height, const std::vector<VkRenderingAttachmentInfo>& colorAttachments, const VkRenderingAttachmentInfo* depthAttachment);

    VkViewport                      CreateViewport(float width, float height);
    VkRect2D                        CreateScissor(uint32_t width, uint32_t height);

    struct ImageBarrierInfo
    {
        VkImageMemoryBarrier barrier;
        VkPipelineStageFlags srcStage;
        VkPipelineStageFlags dstStage;
    };
    ImageBarrierInfo                CreateImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);

    VkSemaphoreCreateInfo           SemaphoreCreateInfo();
    VkFenceCreateInfo               FenceCreateInfo(bool signaled);

    VkImageCreateInfo               ImageCreateInfo(uint32_t width, uint32_t height, VkFormat format, VkImageUsageFlags usage);
    VkImageViewCreateInfo           ImageViewCreateInfo(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    VkSubmitInfo                    SubmitInfo(const VkCommandBuffer cmd, const VkSemaphore wait, const VkPipelineStageFlags waitStage, const VkSemaphore signal);
    VkPresentInfoKHR                PresentInfo(const VkSemaphore& semaphore, VkSwapchainKHR& swapchain, uint32_t& imageIndex);
}

#endif //VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
