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

VkCommandPoolCreateInfo core::gpu_details::CommandPoolCreateInfo(uint32_t graphicsQueueFamily)
{
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsQueueFamily;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    
    return poolInfo;
}

VkCommandBufferAllocateInfo core::gpu_details::CommandBufferAllocateInfo(VkCommandPool pool)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    return allocInfo;
}

VkCommandBufferBeginInfo core::gpu_details::CommandBufferBeginInfo()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    return beginInfo;
}

VkRenderingAttachmentInfo core::gpu_details::CreateRenderingAttachmentInfo(VkImageView imageView, core::rhi::LoadOp loadOp, core::rhi::StoreOp storeOp, const VkClearColorValue& clearValue)
{
    VkRenderingAttachmentInfo vkAttachmentInfo{};
    vkAttachmentInfo.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    vkAttachmentInfo.imageView = imageView;
    vkAttachmentInfo.imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    vkAttachmentInfo.loadOp = (loadOp == core::rhi::LoadOp::Clear) ? VK_ATTACHMENT_LOAD_OP_CLEAR : VK_ATTACHMENT_LOAD_OP_LOAD;
    vkAttachmentInfo.storeOp = (storeOp == core::rhi::StoreOp::Store) ? VK_ATTACHMENT_STORE_OP_STORE : VK_ATTACHMENT_STORE_OP_DONT_CARE;
    vkAttachmentInfo.clearValue.color = clearValue;

    return vkAttachmentInfo;
}

VkRenderingAttachmentInfo core::gpu_details::CreateDepthAttachmentInfo(VkImageView depthView)
{
    VkRenderingAttachmentInfo depthAttachment{};
    depthAttachment.sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
    depthAttachment.imageView = depthView;
    depthAttachment.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.clearValue.depthStencil = { 1.0f, 0 };

    return depthAttachment;
}

VkRenderingInfo core::gpu_details::CreateRenderingInfo(uint32_t width, uint32_t height, const std::vector<VkRenderingAttachmentInfo>& colorAttachments, const VkRenderingAttachmentInfo* depthAttachment)
{
    VkRenderingInfo vkInfo{};
    vkInfo.sType = VK_STRUCTURE_TYPE_RENDERING_INFO;
    vkInfo.renderArea.extent.width = width;
    vkInfo.renderArea.extent.height = height;
    vkInfo.layerCount = 1;
    vkInfo.colorAttachmentCount = static_cast<uint32_t>(colorAttachments.size());
    vkInfo.pColorAttachments = colorAttachments.data();
    vkInfo.pDepthAttachment = depthAttachment;

    return vkInfo;
}

VkViewport core::gpu_details::CreateViewport(float width, float height)
{
    VkViewport viewport{};
    viewport.x = 0.f;
    viewport.y = 0.f;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.f;
    viewport.maxDepth = 1.f;

    return viewport;
}

VkRect2D core::gpu_details::CreateScissor(uint32_t width, uint32_t height)
{
    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = { width, height };

    return scissor;
}

core::gpu_details::ImageBarrierInfo core::gpu_details::CreateImageMemoryBarrier(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    core::gpu_details::ImageBarrierInfo info{};

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    VkPipelineStageFlags dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR &&
        newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
        newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dstStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL &&
        newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
    {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = 0;
        srcStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dstStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }

    info.barrier = barrier;
    info.srcStage = srcStage;
    info.dstStage = dstStage;

    return info;
}