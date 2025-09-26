#ifndef VRAKTAL_CORE_RHI_BUFFER_VK_H
#define VRAKTAL_CORE_RHI_BUFFER_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <core/rhi/buffer.h>

namespace core::rhi
{
	VkBufferUsageFlags ToVkBufferUsage(BufferUsage usage);
	VkMemoryPropertyFlags ToVkMemoryUsage(MemoryUsage memory);

	struct Buffer::Impl
	{
        VkDevice device = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        size_t size = 0;

        Impl(VkDevice dev, VkPhysicalDevice phys, const BufferDesc& desc);
        ~Impl();

        void* Map();
        void Unmap();
        void Update(const void* data, size_t size, size_t offset);
	};
}

#endif //VRAKTAL_CORE_RHI_BUFFER_VK_H
