#include "../gpu_details/converter_vulkan.h"

VkBufferUsageFlags core::gpu_details::ToVkBufferUsage(core::rhi::BufferUsage usage)
{
    switch (usage)
    {
    case core::rhi::BufferUsage::Vertex: return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    case core::rhi::BufferUsage::Index: return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    case core::rhi::BufferUsage::Uniform: return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    case core::rhi::BufferUsage::Storage: return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    case core::rhi::BufferUsage::TransferSrc: return VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    case core::rhi::BufferUsage::TransferDst: return VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    default: return 0;
    }
}

VkMemoryPropertyFlags core::gpu_details::ToVkMemoryUsage(core::rhi::MemoryUsage memory)
{
    switch (memory)
    {
    case core::rhi::MemoryUsage::CPU: return VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    case core::rhi::MemoryUsage::GPU: return VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    default: return 0;
    }
}
