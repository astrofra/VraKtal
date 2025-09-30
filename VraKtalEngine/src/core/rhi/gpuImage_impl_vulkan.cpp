#include "../src/core/rhi/gpuImage_impl_vulkan.h"

#include <stdexcept>
#include <cstring>

using namespace core::rhi;

GpuImage::GpuImage(const GpuImageDesc& desc)
{
    m_impl = std::make_unique<Impl>(desc);
}

GpuImage::~GpuImage() = default;

uint32_t GpuImage::Width() const { return m_impl->Width(); }
uint32_t GpuImage::Height() const { return m_impl->Height(); }

void GpuImage::GetDescriptorInfo(void* outInfo) const
{
    VkDescriptorImageInfo* info = reinterpret_cast<VkDescriptorImageInfo*>(outInfo);
    *info = m_impl->GetDescriptorInfo(VK_NULL_HANDLE); 
}

GpuImage::Impl::Impl(const GpuImageDesc& desc)
{
    extern VkDevice vulkanDevice;
    m_device = vulkanDevice;

    m_width = desc.width;
    m_height = desc.height;
    m_format = VK_FORMAT_R8G8B8A8_UNORM;
}

GpuImage::Impl::~Impl()
{
    if (m_view != VK_NULL_HANDLE)
        vkDestroyImageView(m_device, m_view, nullptr);
    if (m_image != VK_NULL_HANDLE)
        vkDestroyImage(m_device, m_image, nullptr);
    if (m_memory != VK_NULL_HANDLE)
        vkFreeMemory(m_device, m_memory, nullptr);
}

VkDescriptorImageInfo GpuImage::Impl::GetDescriptorInfo(VkSampler sampler) const
{
    VkDescriptorImageInfo info{};
    info.imageLayout = m_currentLayout;
    info.imageView = m_view;
    info.sampler = sampler;
    return info;
}
