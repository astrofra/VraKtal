#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <core/rhi/descriptorSet.h>

namespace core::rhi
{
    VkDescriptorType ToVkDescriptorType(DescriptorType type);
    VkShaderStageFlags ToVkShaderStage(ShaderStage stage);

    struct DescriptorSetLayout::Impl
    {
        Impl(VkDevice device, const DescriptorSetLayoutDesc& desc);
        ~Impl();

        VkDevice device;
        VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    };

    struct DescriptorSet::Impl
    {
        Impl(VkDevice device, VkDescriptorPool pool, DescriptorSetLayout* layout);
        ~Impl();

        void BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range);
        void BindImage(uint32_t binding, Image* image, const ImageUsage& usage); // TODO : Implement this later.

        VkDevice device;
        VkDescriptorSet set = VK_NULL_HANDLE;
        DescriptorSetLayout* layout = nullptr;
    };
}

#endif //VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
