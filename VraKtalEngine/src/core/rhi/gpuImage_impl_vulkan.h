#ifndef VRAKTAL_CORE_RHI_GPU_IMAGE_VK_H
#define VRAKTAL_CORE_RHI_GPU_IMAGE_VK_H
#pragma once

#include <core/rhi/gpuImage.h>
#include <vulkan/vulkan.h>

namespace core::rhi
{
    struct GpuImage::Impl
    {
        Impl(GpuDevice& device, const GpuImageDesc& desc);

        Impl(VkDevice device, VkImage image, VkImageView view, VkFormat format, uint32_t width, uint32_t height, bool ownsImage = true);

        ~Impl();

        uint32_t Width() const { return m_width; }
        uint32_t Height() const { return m_height; }

        VkImage GetNative() const { return m_image; }
        VkImageView GetView() const { return m_view; }
        VkFormat Format() const { return m_format; }

        VkImageLayout CurrentLayout() const { return m_currentLayout; }
        void SetLayout(VkImageLayout layout) const { m_currentLayout = layout; }

        VkDescriptorImageInfo GetDescriptorInfo(VkSampler sampler) const;

        VkDevice m_device = VK_NULL_HANDLE;
        VkImage m_image = VK_NULL_HANDLE;
        VkImageView m_view = VK_NULL_HANDLE;
        VkDeviceMemory m_memory = VK_NULL_HANDLE;
        VkFormat m_format = VK_FORMAT_UNDEFINED;
        uint32_t m_width = 0, m_height = 0;

        mutable VkImageLayout m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        bool m_ownsImage = true;
    };
}

#endif // VRAKTAL_CORE_RHI_GPU_IMAGE_VK_H
