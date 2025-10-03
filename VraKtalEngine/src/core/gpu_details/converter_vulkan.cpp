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

VkDescriptorType core::gpu_details::ToVkDescriptorType(core::rhi::DescriptorType type)
{
    switch (type)
    {
    case core::rhi::DescriptorType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    case core::rhi::DescriptorType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    case core::rhi::DescriptorType::ImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    default: return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

VkShaderStageFlags core::gpu_details::ToVkShaderStage(core::rhi::ShaderStage stage)
{
    switch (stage)
    {
    case core::rhi::ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case core::rhi::ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case core::rhi::ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    default: return 0;
    }
}
