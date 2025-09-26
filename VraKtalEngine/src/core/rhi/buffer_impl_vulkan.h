#ifndef VRAKTAL_CORE_RHI_BUFFER_VK_H
#define VRAKTAL_CORE_RHI_BUFFER_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <core/rhi/buffer.h>

using namespace core::rhi;

namespace core::rhi::vulkan
{
	VkBufferUsageFlags ToVkBufferUsage(BufferUsage usage);
	VkMemoryPropertyFlags ToVkMemoryUsage(MemoryUsage memory);

	class BufferVulkan : public Buffer
	{
	public:
		BufferVulkan(VkDevice device, VkPhysicalDevice physicalDevice, const BufferDesc& desc);
		~BufferVulkan() override;

		void* Map() override;
		void Unmap() override;
		void Update(const void* data, size_t size, size_t offset = 0) override;

		VkBuffer GetHandle() const { return m_buffer; }
		VkDeviceMemory GetMemory() const { return m_memory; }

	private:
		VkDevice m_device;
		VkBuffer m_buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_memory = VK_NULL_HANDLE;
		size_t m_size = 0;
	};
}

#endif //VRAKTAL_CORE_RHI_BUFFER_VK_H
