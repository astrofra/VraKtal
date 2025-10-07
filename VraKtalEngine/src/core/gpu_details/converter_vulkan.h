#ifndef VRAKTAL_CORE_GPU_DETAILS_CONVERTER_VK_H
#define VRAKTAL_CORE_GPU_DETAILS_CONVERTER_VK_H
#pragma once

#include <core/rhi/buffer.h>
#include <core/rhi/descriptorSet.h>
#include <core/rhi/enums.h>

#include <vulkan/vulkan.h>

namespace core::gpu_details
{
	VkBufferUsageFlags		ToVkBufferUsage(core::rhi::BufferUsage usage);
	VkMemoryPropertyFlags	ToVkMemoryUsage(core::rhi::MemoryUsage memory);
	VkDescriptorType		ToVkDescriptorType(core::rhi::DescriptorType type);
	VkShaderStageFlags		ToVkShaderStage(core::rhi::ShaderStage stage);
	VkFormat				ToVkFormat(core::rhi::Format format);
	VkImageUsageFlags		ToVkImageUsage(core::rhi::TextureUsage usage);
	VkImageLayout			ToVkImageLayout(core::rhi::TextureLayout layout);
}

#endif //VRAKTAL_CORE_GPU_DETAILS_CONVERTER_VK_H