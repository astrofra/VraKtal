#include "../src/core/rhi/descriptorSet_impl_vulkan.h"
#include "../src/core/rhi/buffer_impl_vulkan.h"
#include "../src/core/rhi/gpuImage_impl_vulkan.h"

#include "../src/core/gpu_details/converter_vulkan.h"
#include "../src/core/gpu_details/initializer_vulkan.h"

#include <stdexcept>

using namespace core::rhi;

DescriptorSet::Impl::Impl(VkDevice device, VkDescriptorPool pool, const DescriptorSetLayoutDesc& desc)
    : m_device(device)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.reserve(desc.bindings.size());

    for (auto& b : desc.bindings)
    {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = b.binding;
        layoutBinding.descriptorType = core::gpu_details::ToVkDescriptorType(b.type);
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = core::gpu_details::ToVkShaderStage(b.stage);
        layoutBinding.pImmutableSamplers = nullptr;
        bindings.push_back(layoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo = core::gpu_details::DescriptorSetLayoutCreateInfo(bindings);

    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan descriptor set layout");
    }
        
    VkDescriptorSetAllocateInfo allocInfo = core::gpu_details::DescriptorSetCreateAllocateInfo(m_layout, pool);

    if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_set) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to allocate Vulkan descriptor set");
    }        
}

DescriptorSet::Impl::~Impl()
{
    if (m_layout != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
    }       
}

void DescriptorSet::Impl::BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range)
{
    VkDescriptorBufferInfo bufferInfo = core::gpu_details::DescriptorSetCreateBufferInfo(buffer->GetImpl().buffer, offset, range);
    VkWriteDescriptorSet write = core::gpu_details::DescriptorSetCreateWriteBuffer(m_set, binding, &bufferInfo);

    vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);
}

void DescriptorSet::Impl::BindImage(uint32_t binding, GpuImage* image, const GpuImageUsage& usage)
{
    auto* vkImage = dynamic_cast<GpuImage*>(image);
    if (!vkImage)
    {
        throw std::runtime_error("BindImage: image is not a Vulkan GpuImage");
    }
        
    VkDescriptorImageInfo imageInfo = core::gpu_details::DescriptorSetCreateImageInfo(vkImage->GetImpl().GetView(), vkImage->GetImpl().CurrentLayout(), VK_NULL_HANDLE);
    VkWriteDescriptorSet write = core::gpu_details::DescriptorSetCreateWriteImage(m_set, binding, &imageInfo);

    vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);
}

DescriptorSet::DescriptorSet(VkDevice device, VkDescriptorPool pool, const DescriptorSetLayoutDesc& layoutDesc)
    : m_impl(std::make_unique<Impl>(device, pool, layoutDesc))
{
}

DescriptorSet::~DescriptorSet() = default;

void DescriptorSet::BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range)
{
    m_impl->BindBuffer(binding, buffer, offset, range);
}

void DescriptorSet::BindImage(uint32_t binding, GpuImage* image, const GpuImageUsage& usage)
{
    m_impl->BindImage(binding, image, usage);
}

DescriptorSet::Impl& DescriptorSet::GetImpl() { return *m_impl; }
