#include "../gpu_details/converter_vulkan.h"

#include <core/rhi/enums.h>

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

VkFormat core::gpu_details::ToVkFormat(core::rhi::Format format)
{
    switch (format)
    {
    case core::rhi::Format::R8_UNorm:           return VK_FORMAT_R8_UNORM;
    case core::rhi::Format::RG8_UNorm:          return VK_FORMAT_R8G8_UNORM;
    case core::rhi::Format::RGB8_UNorm:         return VK_FORMAT_R8G8B8_UNORM;
    case core::rhi::Format::RGBA8_UNorm:        return VK_FORMAT_R8G8B8A8_UNORM;
    case core::rhi::Format::BGRA8_UNorm:        return VK_FORMAT_B8G8R8A8_UNORM;
    case core::rhi::Format::RGBA8_SRGB:         return VK_FORMAT_R8G8B8A8_SRGB;
         
    case core::rhi::Format::R16_Float:          return VK_FORMAT_R16_SFLOAT;
    case core::rhi::Format::RG16_Float:         return VK_FORMAT_R16G16_SFLOAT;
    case core::rhi::Format::RGBA16_Float:       return VK_FORMAT_R16G16B16A16_SFLOAT;
    case core::rhi::Format::R32_Float:          return VK_FORMAT_R32_SFLOAT;
    case core::rhi::Format::RG32_Float:         return VK_FORMAT_R32G32_SFLOAT;
    case core::rhi::Format::RGBA32_Float:       return VK_FORMAT_R32G32B32A32_SFLOAT;
         
    case core::rhi::Format::D16_UNorm:          return VK_FORMAT_D16_UNORM;
    case core::rhi::Format::D24_UNorm_S8_UInt:  return VK_FORMAT_D24_UNORM_S8_UINT;
    case core::rhi::Format::D32_Float:          return VK_FORMAT_D32_SFLOAT;
    case core::rhi::Format::D32_Float_S8_UInt:  return VK_FORMAT_D32_SFLOAT_S8_UINT;

    default:                                    return VK_FORMAT_UNDEFINED;
    }
}

VkImageUsageFlags core::gpu_details::ToVkImageUsage(core::rhi::TextureUsage usage)
{
    VkImageUsageFlags flags = 0;

    if (usage& core::rhi::TextureUsage::TransferSrc)            flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    if (usage& core::rhi::TextureUsage::TransferDst)            flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (usage& core::rhi::TextureUsage::Sampled)                flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (usage& core::rhi::TextureUsage::Storage)                flags |= VK_IMAGE_USAGE_STORAGE_BIT;
    if (usage& core::rhi::TextureUsage::ColorAttachment)        flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (usage& core::rhi::TextureUsage::DepthStencilAttachment) flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (usage& core::rhi::TextureUsage::InputAttachment)        flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

    return flags;
}

VkImageLayout core::gpu_details::ToVkImageLayout(core::rhi::TextureLayout layout)
{
    switch (layout)
    {
    case core::rhi::TextureLayout::Undefined:               return VK_IMAGE_LAYOUT_UNDEFINED;
    case core::rhi::TextureLayout::General:                 return VK_IMAGE_LAYOUT_GENERAL;
    case core::rhi::TextureLayout::ColorAttachment:         return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case core::rhi::TextureLayout::DepthStencilAttachment:  return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case core::rhi::TextureLayout::ShaderReadOnly:          return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case core::rhi::TextureLayout::TransferSrc:             return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case core::rhi::TextureLayout::TransferDst:             return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case core::rhi::TextureLayout::PresentSrc:              return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    default:                                                return VK_IMAGE_LAYOUT_UNDEFINED;
    }
}