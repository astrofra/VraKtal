#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
#pragma once

#include <vulkan/vulkan.h>
#include <core/rhi/descriptorSet.h>

namespace core::rhi
{
    struct DescriptorSet::Impl
    {
        Impl(VkDevice device, VkDescriptorPool pool, const DescriptorSetLayoutDesc& desc);
        ~Impl();

        void BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range);
        void BindImage(uint32_t binding, GpuImage* image, const GpuImageUsage& usage);

        VkDevice m_device = VK_NULL_HANDLE;
        VkDescriptorSet m_set = VK_NULL_HANDLE;
        VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
    };
}

#endif // VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
