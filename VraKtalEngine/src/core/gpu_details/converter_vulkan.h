#ifndef VRAKTAL_CORE_GPU_DETAILS_CONVERTER_VK_H
#define VRAKTAL_CORE_GPU_DETAILS_CONVERTER_VK_H
#pragma once

#include <core/rhi/buffer.h>
#include <vulkan/vulkan.h>

namespace core::gpu_details
{
	VkBufferUsageFlags ToVkBufferUsage(core::rhi::BufferUsage usage);
	VkMemoryPropertyFlags ToVkMemoryUsage(core::rhi::MemoryUsage memory);
}

#endif //VRAKTAL_CORE_GPU_DETAILS_CONVERTER_VK_H