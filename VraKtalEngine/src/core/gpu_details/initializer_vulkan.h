#ifndef VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
#define VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include <core/rhi/buffer.h>
#include <core/rhi/renderingInfo.h>

#include "converter_vulkan.h"

namespace core::gpu_details
{
	struct ImageBarrierInfo
	{
		VkImageMemoryBarrier barrier;
		VkPipelineStageFlags srcStage;
		VkPipelineStageFlags dstStage;
	};

	VkBufferCreateInfo					BufferCreateInfo(size_t size, core::rhi::BufferUsage usage);
	VkMemoryAllocateInfo				BufferCreateAllocateInfo(uint64_t allocationSize, uint32_t size);
	VkDescriptorSetLayoutCreateInfo		DescriptorSetLayoutCreateInfo(std::vector<VkDescriptorSetLayoutBinding> bindings);
	VkDescriptorSetAllocateInfo			DescriptorSetCreateAllocateInfo(VkDescriptorSetLayout layout, VkDescriptorPool pool);
	VkDescriptorBufferInfo				DescriptorSetCreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);
	VkWriteDescriptorSet				DescriptorSetCreateWriteBuffer(VkDescriptorSet set, uint32_t binding, const VkDescriptorBufferInfo* bufferInfo);
	VkDescriptorImageInfo				DescriptorSetCreateImageInfo(VkImageView imageView, VkImageLayout layout, VkSampler sampler);
	VkWriteDescriptorSet				DescriptorSetCreateWriteImage(VkDescriptorSet set, uint32_t binding, const VkDescriptorImageInfo* imageInfo);
	VkCommandPoolCreateInfo				CommandPoolCreateInfo(uint32_t graphicsQueueFamily);
	VkCommandBufferAllocateInfo			CommandBufferAllocateInfo(VkCommandPool pool);
	VkCommandBufferBeginInfo			CommandBufferBeginInfo();
	VkRenderingAttachmentInfo			CreateRenderingAttachmentInfo(VkImageView imageView, core::rhi::LoadOp loadOp, core::rhi::StoreOp storeOp, const VkClearColorValue& clearValue);
	VkRenderingAttachmentInfo			CreateDepthAttachmentInfo(VkImageView depthView);
	VkRenderingInfo						CreateRenderingInfo(uint32_t width, uint32_t height, const std::vector<VkRenderingAttachmentInfo>& colorAttachments, const VkRenderingAttachmentInfo* depthAttachment);
	VkViewport							CreateViewport(float width, float height);
	VkRect2D							CreateScissor(uint32_t width, uint32_t height);
	core::gpu_details::ImageBarrierInfo	CreateImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
}
#endif //VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
