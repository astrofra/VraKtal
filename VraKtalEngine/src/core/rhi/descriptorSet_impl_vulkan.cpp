#include "../src/core/rhi/descriptorSet_impl_vulkan.h"
#include "../src/core/rhi/buffer_impl_vulkan.h"

#include <stdexcept>

using namespace core::rhi::vulkan;

VkDescriptorType core::rhi::vulkan::ToVkDescriptorType(DescriptorType type)
{
	switch (type)
	{
		case DescriptorType::UniformBuffer: return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case DescriptorType::StorageBuffer: return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case DescriptorType::ImageSampler: return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		default: return VK_DESCRIPTOR_TYPE_MAX_ENUM;
	}
}

VkShaderStageFlags core::rhi::vulkan::ToVkShaderStage(ShaderStage stage)
{
	switch (stage)
	{
		case ShaderStage::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
		case ShaderStage::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
		case ShaderStage::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
		default: return 0;
	}
}

DescriptorSetLayoutVulkan::DescriptorSetLayoutVulkan(VkDevice device, const DescriptorSetLayoutDesc& desc)
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
		throw std::runtime_error("Failed to create Vulkan desciptor set layout");
	}
}

DescriptorSetLayoutVulkan::~DescriptorSetLayoutVulkan()
{
	if (m_layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
	};
}

DescriptorSetVulkan::DescriptorSetVulkan(VkDevice device, VkDescriptorPool pool, DescriptorSetLayoutVulkan* layout)
	: m_device(device), m_layout(layout)
{
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	VkDescriptorSetLayout layouts[] = { layout->GetHandle() };
	allocInfo.pSetLayouts = layouts;

	if (vkAllocateDescriptorSets(m_device, &allocInfo, &m_set) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Vulkan descriptor set");
	}
}

DescriptorSetVulkan::~DescriptorSetVulkan()
{

}

void DescriptorSetVulkan::BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range)
{
	auto* vkBuffer = dynamic_cast<BufferVulkan*>(buffer);
	
	if(!vkBuffer)
	{
		throw std::runtime_error("BindBuffer: buffer is not a Vulkan Buffer");
	}

	VkDescriptorBufferInfo bufferInfo{};
	bufferInfo.buffer = vkBuffer->GetHandle();
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