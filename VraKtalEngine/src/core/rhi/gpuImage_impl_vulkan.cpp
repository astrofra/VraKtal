#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"
#include "../src/core/rhi/gpuImage_impl_vulkan.h"

using namespace core::rhi;

GpuImage::Impl::Impl(GpuDevice& device, const GpuImageDesc& desc)
{
    m_device = device.GetImpl().m_device;
    m_width = desc.width;
    m_height = desc.height;
    m_format = VK_FORMAT_R8G8B8A8_UNORM;
}

GpuImage::Impl::Impl(VkDevice device, VkImage image, VkImageView view, VkFormat format,
    uint32_t width, uint32_t height, bool ownsImage)
    : m_device(device), m_image(image), m_view(view), m_format(format),
    m_width(width), m_height(height), m_ownsImage(ownsImage)
{
    m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
}

GpuImage::Impl::~Impl()
{
    if (!m_ownsImage) return;

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

GpuImage::GpuImage(GpuDevice& device, const GpuImageDesc& desc)
{
    m_impl = std::make_unique<Impl>(device, desc);
}

GpuImage::GpuImage(std::unique_ptr<Impl> impl)
    : m_impl(std::move(impl))
{
}

GpuImage::~GpuImage() = default;

uint32_t GpuImage::Width() const { return m_impl->Width(); }
uint32_t GpuImage::Height() const { return m_impl->Height(); }

void GpuImage::GetDescriptorInfo(void* outInfo, void* sampler) const
{
    VkDescriptorImageInfo* info = reinterpret_cast<VkDescriptorImageInfo*>(outInfo);
    VkSampler vkSampler = sampler ? *reinterpret_cast<VkSampler*>(sampler) : VK_NULL_HANDLE;
    *info = m_impl->GetDescriptorInfo(vkSampler);
}

GpuImage::Impl& GpuImage::GetImpl() { return *m_impl; }
