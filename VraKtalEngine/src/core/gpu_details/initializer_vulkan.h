#ifndef VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
#define VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include <core/rhi/buffer.h>

#include "converter_vulkan.h"

namespace core::gpu_details
{
	VkBufferCreateInfo					BufferCreateInfo(size_t size, core::rhi::BufferUsage usage);
	VkMemoryAllocateInfo				BufferCreateAllocateInfo(uint64_t allocationSize, uint32_t size);
	VkDescriptorSetLayoutCreateInfo		DescriptorSetLayoutCreateInfo(std::vector<VkDescriptorSetLayoutBinding> bindings);
	VkDescriptorSetAllocateInfo			DescriptorSetCreateAllocateInfo(VkDescriptorSetLayout layout, VkDescriptorPool pool);
	VkDescriptorBufferInfo				DescriptorSetCreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range);
	VkWriteDescriptorSet				DescriptorSetCreateWriteBuffer(VkDescriptorSet set, uint32_t binding, const VkDescriptorBufferInfo* bufferInfo);
	VkDescriptorImageInfo				DescriptorSetCreateImageInfo(VkImageView imageView, VkImageLayout layout, VkSampler sampler);
	VkWriteDescriptorSet				DescriptorSetCreateWriteImage(VkDescriptorSet set, uint32_t binding, const VkDescriptorImageInfo* imageInfo);

}

#endif //VRAKTAL_CORE_GPU_DETAILS_INITIALIZER_VK_H
