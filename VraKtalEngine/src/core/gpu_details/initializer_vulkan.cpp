#include "initializer_vulkan.h"

VkBufferCreateInfo core::gpu_details::BufferCreateInfo(size_t size, core::rhi::BufferUsage usage)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = gpu_details::ToVkBufferUsage(usage);
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    return bufferInfo;
}

VkMemoryAllocateInfo core::gpu_details::BufferCreateAllocateInfo(uint64_t allocationSize, uint32_t size)
{
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = size;
    allocInfo.memoryTypeIndex = allocationSize;

    return allocInfo;
}

VkDescriptorSetLayoutCreateInfo core::gpu_details::DescriptorSetLayoutCreateInfo(std::vector<VkDescriptorSetLayoutBinding> bindings)
{
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    return layoutInfo;
}

VkDescriptorSetAllocateInfo core::gpu_details::DescriptorSetCreateAllocateInfo(VkDescriptorSetLayout layout, VkDescriptorPool pool)
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = pool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &layout;

    return allocInfo;
}

VkDescriptorBufferInfo core::gpu_details::DescriptorSetCreateBufferInfo(VkBuffer buffer, VkDeviceSize offset, VkDeviceSize range)
{
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = buffer;
    bufferInfo.offset = offset;
    bufferInfo.range = range;

    return bufferInfo;
}

VkWriteDescriptorSet core::gpu_details::DescriptorSetCreateWriteBuffer(VkDescriptorSet set, uint32_t binding, const VkDescriptorBufferInfo* bufferInfo)
{
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = set;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    write.descriptorCount = 1;
    write.pBufferInfo = bufferInfo;

    return write;
}

VkDescriptorImageInfo core::gpu_details::DescriptorSetCreateImageInfo(VkImageView imageView, VkImageLayout layout, VkSampler sampler)
{
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = layout;
    imageInfo.imageView = imageView;
    imageInfo.sampler = sampler;

    return imageInfo;
}

VkWriteDescriptorSet core::gpu_details::DescriptorSetCreateWriteImage(VkDescriptorSet set, uint32_t binding, const VkDescriptorImageInfo* imageInfo)
{
    VkWriteDescriptorSet write{};
    write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    write.dstSet = set;
    write.dstBinding = binding;
    write.dstArrayElement = 0;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.descriptorCount = 1;
    write.pImageInfo = imageInfo;

    return write;
}