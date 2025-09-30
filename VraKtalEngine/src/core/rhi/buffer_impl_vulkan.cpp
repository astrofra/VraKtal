#include "../src/core/rhi/buffer_impl_vulkan.h"
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"

#include <stdexcept>
#include <cstring>

using namespace core::rhi;

Buffer::Buffer(const BufferDesc& desc, GpuDevice& device)
    : m_desc(desc)
{
    m_impl = std::make_unique<Impl>(device, desc);
}

Buffer::~Buffer() = default;
Buffer::Impl& Buffer::GetImpl() { return *m_impl; }

void* Buffer::Map() { return m_impl->Map(); }
void Buffer::Unmap() { m_impl->Unmap(); }
void Buffer::Update(const void* data, size_t size, size_t offset) { m_impl->Update(data, size, offset); }

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
    case MemoryUsage::CPU: return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    case MemoryUsage::GPU: return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    default: return 0;
    }
}

Buffer::Impl::Impl(GpuDevice& gpuDevice, const BufferDesc& desc)
{
    device = gpuDevice.GetImpl().m_device;
    physicalDevice = gpuDevice.GetImpl().m_physicalDevice;
    size = desc.size;

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = desc.size;
    bufferInfo.usage = ToVkBufferUsage(desc.usage);
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        throw std::runtime_error("Failed to create Vulkan buffer");

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    uint32_t memoryTypeIndex = UINT32_MAX;
    VkMemoryPropertyFlags wantedFlags = ToVkMemoryUsage(desc.memory);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if ((memRequirements.memoryTypeBits & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & wantedFlags) == wantedFlags)
        {
            memoryTypeIndex = i;
            break;
        }
    }

    if (memoryTypeIndex == UINT32_MAX)
        throw std::runtime_error("Failed to find suitable memory type for buffer");

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = memoryTypeIndex;

    if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        throw std::runtime_error("Failed to allocate Vulkan buffer memory");

    vkBindBufferMemory(device, buffer, memory, 0);
}

Buffer::Impl::~Impl()
{
    if (buffer != VK_NULL_HANDLE) vkDestroyBuffer(device, buffer, nullptr);
    if (memory != VK_NULL_HANDLE) vkFreeMemory(device, memory, nullptr);
}

void* Buffer::Impl::Map()
{
    void* data = nullptr;
    vkMapMemory(device, memory, 0, size, 0, &data);
    return data;
}

void Buffer::Impl::Unmap()
{
    vkUnmapMemory(device, memory);
}

void Buffer::Impl::Update(const void* data, size_t updateSize, size_t offset)
{
    void* mappedData = nullptr;
    vkMapMemory(device, memory, offset, updateSize, 0, &mappedData);
    std::memcpy(mappedData, data, updateSize);
    vkUnmapMemory(device, memory);
}
