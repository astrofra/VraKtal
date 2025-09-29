#include "../src/core/rhi/buffer_impl_vulkan.h"

#include <stdexcept>
#include <cstring>

using namespace core::rhi;

void* Buffer::Map() { return m_impl->Map(); }
void Buffer::Unmap() { m_impl->Unmap(); }
void Buffer::Update(const void* data, size_t size, size_t offset) { m_impl->Update(data, size, offset); }

Buffer::Buffer(const BufferDesc& desc)
{
	extern VkDevice vulkanDevice;
	extern VkPhysicalDevice vulkanPhysicalDevice;
	m_impl = std::make_unique<Impl>(vulkanDevice, vulkanPhysicalDevice, desc);
}

Buffer::~Buffer() = default;

void Buffer::GetDescriptorInfo(size_t offset, size_t range, void* outInfo) const
{
	VkDescriptorBufferInfo* info = reinterpret_cast<VkDescriptorBufferInfo*>(outInfo);
	info->buffer = m_impl->buffer;
	info->offset = offset;
	info->range = range;
}

VkBufferUsageFlags core::rhi::ToVkBufferUsage(BufferUsage usage)
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

VkMemoryPropertyFlags core::rhi::ToVkMemoryUsage(MemoryUsage memory)
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

Buffer::Impl::Impl(VkDevice device, VkPhysicalDevice physicalDevice, const BufferDesc& desc)
	: device(device), physicalDevice(physicalDevice), size(desc.size)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = desc.size;
	bufferInfo.usage = ToVkBufferUsage(desc.usage);
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan Buffer");
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

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

	if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Vulkan Buffer memory");
	}

	vkBindBufferMemory(device, buffer, memory, 0);
}

Buffer::Impl::~Impl()
{
	if (buffer != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(device, buffer, nullptr);
	}
	if (memory != VK_NULL_HANDLE)
	{
		vkFreeMemory(device, memory, nullptr);
	}
}

void* Buffer::Impl::Map()
{
	void* data;
	vkMapMemory(device, memory, 0, size, 0, &data);
	return data;
}

void Buffer::Impl::Unmap()
{
	vkUnmapMemory(device, memory);
}

void Buffer::Impl::Update(const void* data, size_t size, size_t offset)
{
	void* mappedData = nullptr;
	vkMapMemory(device, memory, offset, size, 0, &mappedData);
	std::memcpy(mappedData, data, size);
	vkUnmapMemory(device, memory);
}
