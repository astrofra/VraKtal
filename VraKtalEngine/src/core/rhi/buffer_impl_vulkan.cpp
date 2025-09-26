#include "../src/core/rhi/buffer_impl_vulkan.h"

#include <stdexcept>
#include <cstring>

using namespace core::rhi::vulkan;

VkBufferUsageFlags core::rhi::vulkan::ToVkBufferUsage(BufferUsage usage)
{
	switch (usage)
	{
		case BufferUsage::Vertex: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		case BufferUsage::Index: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		case BufferUsage::Uniform: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		case BufferUsage::Storage: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		case BufferUsage::TransferSrc: return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		case BufferUsage::TransferDst: return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		default: return 0;
	}
}

VkMemoryPropertyFlags core::rhi::vulkan::ToVkMemoryUsage(MemoryUsage memory)
{
	switch (memory)
	{
		case MemoryUsage::CPU:
			return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		case MemoryUsage::GPU:
			return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		default:
			return 0;
	}
}

BufferVulkan::BufferVulkan(VkDevice device, VkPhysicalDevice physicalDevice, const BufferDesc& desc)
	: m_device(device), m_size(desc.size)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = desc.size;
	bufferInfo.usage = ToVkBufferUsage(desc.usage);
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(m_device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan Buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(m_device, m_buffer, &memRequirements);

	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

	uint32_t memoryTypeIndex = UINT32_MAX;
	VkMemoryPropertyFlags wantedFlags = ToVkMemoryUsage(desc.memory);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((memRequirements.memoryTypeBits & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & wantedFlags) == wantedFlags)
		{
			memoryTypeIndex = i;
			break;
		}
	}

	if (memoryTypeIndex == UINT32_MAX)
	{
		throw std::runtime_error("Failed to find suitable memory type for buffer");
	}

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = memoryTypeIndex;

	if (vkAllocateMemory(m_device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Vulkan Buffer memory");
	}

	vkBindBufferMemory(m_device, m_buffer, m_memory, 0);
}

BufferVulkan::~BufferVulkan()
{
	if (m_buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_device, m_buffer, nullptr);
	}
	if (m_memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(m_device, m_memory, nullptr);
	}
}

void* BufferVulkan::Map()
{
	void* data;
	vkMapMemory(m_device, m_memory, 0, m_size, 0, &data);
	return data;
}

void BufferVulkan::Unmap()
{
	vkUnmapMemory(m_device, m_memory);
}

void BufferVulkan::Update(const void* data, size_t size, size_t offset)
{
	void* mappedData = nullptr;
	vkMapMemory(m_device, m_memory, offset, size, 0, &mappedData);
	std::memcpy(mappedData, data, size);
	vkUnmapMemory(m_device, m_memory);
}
