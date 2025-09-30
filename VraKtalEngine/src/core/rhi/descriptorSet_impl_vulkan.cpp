#include "../src/core/rhi/descriptorSet_impl_vulkan.h"
#include "../src/core/rhi/buffer_impl_vulkan.h"
#include "../src/core/rhi/gpuImage_impl_vulkan.h"

#include <stdexcept>

using namespace core::rhi;

VkDescriptorType core::rhi::ToVkDescriptorType(DescriptorType type)
{
    switch (type)
    {
        case DescriptorType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case DescriptorType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case DescriptorType::ImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        default: return VK_DESCRIPTOR_TYPE_MAX_ENUM;
    }
}

VkShaderStageFlags core::rhi::ToVkShaderStage(ShaderStage stage)
{
    switch (stage)
    {
        case ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
        case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
        case ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
        default: return 0;
    }
}

DescriptorSet::Impl::Impl(VkDevice device, VkDescriptorPool pool, const DescriptorSetLayoutDesc& desc)
    : m_device(device)
{
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.reserve(desc.bindings.size());

    for (auto& b : desc.bindings)
    {
        VkDescriptorSetLayoutBinding layoutBinding{};
        layoutBinding.binding = b.binding;
        layoutBinding.descriptorType = ToVkDescriptorType(b.type);
        layoutBinding.descriptorCount = 1;
        layoutBinding.stageFlags = ToVkShaderStage(b.stage);
        layoutBinding.pImmutableSamplers = nullptr;
        bindings.push_back(layoutBinding);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_layout) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Vulkan descriptor set layout");
    }
        
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_layout;

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
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer->GetImpl().buffer;
    bufferInfo.offset = offset;
    bufferInfo.range = range;

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = m_set;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.descriptorCount = 1;
    write.pBufferInfo = &bufferInfo;

    vkUpdateDescriptorSets(m_device, 1, &write, 0, nullptr);
}

void DescriptorSet::Impl::BindImage(uint32_t binding, GpuImage* image, const GpuImageUsage& usage)
{
    auto* vkImage = dynamic_cast<GpuImage*>(image);
    if (!vkImage)
    {
        throw std::runtime_error("BindImage: image is not a Vulkan GpuImage");
    }
        
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = vkImage->GetImpl().CurrentLayout();
    imageInfo.imageView = vkImage->GetImpl().GetView();
    imageInfo.sampler = VK_NULL_HANDLE;

    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = m_set;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = &imageInfo;

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
