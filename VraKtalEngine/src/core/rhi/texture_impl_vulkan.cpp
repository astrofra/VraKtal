#include "../src/core/rhi/texture_impl_vulkan.h"
#include "../src/core/rhi/commandBuffer_impl_vulkan.h"
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"
#include "../src/core/gpu_details/converter_vulkan.h"

using namespace core::rhi;

Texture::Texture(GpuDevice* device, TextureDesc& desc)
{
	m_impl = std::make_unique<Impl>(device, desc);
}

Texture::~Texture() = default;
Texture::Impl& Texture::GetImpl() { return *m_impl; }

void Texture::UploadData(const void* data, size_t size, uint32_t mipLevel) { UploadData(data, size, mipLevel); }
void Texture::GenerateMipmaps(CommandBuffer* cmd) { GenerateMipmaps(cmd); }
void Texture::TransitionLayout(CommandBuffer* cmd, TextureLayout newLayout) { TransitionLayout(cmd, newLayout); }

Texture::Impl::Impl(GpuDevice* device, TextureDesc& desc)
	: m_device(device)
{
	CreateImageAndView();
}

Texture::Impl::~Impl()
{
	DestroyImageAndView();
}

void Texture::Impl::CreateImageAndView()
{
    VkDevice dev = m_device->GetImpl().m_device;

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = m_desc.width;
    imageInfo.extent.height = m_desc.height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = m_desc.mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = gpu_details::ToVkFormat(m_desc.format);
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = gpu_details::ToVkImageUsage(m_desc.usage);
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

    if (vkCreateImage(dev, &imageInfo, nullptr, &m_image) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create image");
    }

    VkMemoryRequirements memReq;
    vkGetImageMemoryRequirements(dev, m_image, &memReq);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReq.size;

    if (vkAllocateMemory(dev, &allocInfo, nullptr, &m_memory) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate image memory");
    }

    vkBindImageMemory(dev, m_image, m_memory, 0);

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = m_image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = gpu_details::ToVkFormat(m_desc.format);
    viewInfo.subresourceRange.aspectMask = (m_desc.usage & rhi::TextureUsage::DepthStencilAttachment) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = m_desc.mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(dev, &viewInfo, nullptr, &m_view) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create image view");
    }
}

void Texture::Impl::DestroyImageAndView()
{
    VkDevice dev = m_device->GetImpl().m_device;
    if (m_view != VK_NULL_HANDLE) vkDestroyImageView(dev, m_view, nullptr);
    if (m_image != VK_NULL_HANDLE) vkDestroyImage(dev, m_image, nullptr);
    if (m_memory != VK_NULL_HANDLE) vkFreeMemory(dev, m_memory, nullptr);
    m_view = VK_NULL_HANDLE;
    m_image = VK_NULL_HANDLE;
    m_memory = VK_NULL_HANDLE;
}

void Texture::Impl::UploadData(const void* data, size_t size, uint32_t mipLevel)
{

}

void Texture::Impl::TransitionLayout(CommandBuffer* cmd, TextureLayout newLayout)
{

}

void Texture::Impl::GenerateMipmaps(CommandBuffer* cmd)
{

}


