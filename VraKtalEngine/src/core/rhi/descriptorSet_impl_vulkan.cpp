#include "../src/core/rhi/descriptorSet_impl_vulkan.h"
#include "../src/core/rhi/image_impl_vulkan.h"

#include <core/rhi/buffer.h>

#include <stdexcept>

using namespace core::rhi;

DescriptorSetLayout::DescriptorSetLayout(void* device, const DescriptorSetLayoutDesc& desc)
{
	m_impl = std::make_unique<Impl>(reinterpret_cast<VkDevice>(device), desc);
}

DescriptorSetLayout::~DescriptorSetLayout() = default;

DescriptorSet::DescriptorSet(void* device, void* pool, const DescriptorSetLayoutDesc& layoutDesc)
{
	m_impl = std::make_unique<Impl>(
		reinterpret_cast<VkDevice>(device),
		reinterpret_cast<VkDescriptorPool>(pool),
		layoutDesc
	);
}

DescriptorSet::~DescriptorSet() = default;

void DescriptorSet::BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range)
{
	m_impl->BindBuffer(binding, buffer, offset, range);
}

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

DescriptorSetLayout::Impl::Impl(VkDevice _device, const DescriptorSetLayoutDesc& desc)
	: device(_device)
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

	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan desciptor set layout");
	}
}

DescriptorSetLayout::Impl::~Impl()
{
	if (layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device, layout, nullptr);
	};
}

DescriptorSet::Impl::Impl(VkDevice device, VkDescriptorPool pool, DescriptorSetLayout* layout)
	: device(device), layout(layout)
{
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = 1;
	VkDescriptorSetLayout layouts[] = { layout->m_impl->layout };
	allocInfo.pSetLayouts = layouts;

	if (vkAllocateDescriptorSets(device, &allocInfo, &set) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate Vulkan descriptor set");
	}
}

DescriptorSet::Impl::~Impl()
{

}

void DescriptorSet::Impl::BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range)
{

	if(!buffer)
	{
		throw std::runtime_error("BindBuffer: buffer is not a Vulkan Buffer");
	}

	VkDescriptorBufferInfo bufferInfo{};
	buffer->GetDescriptorInfo(offset, range, &bufferInfo);

	VkWriteDescriptorSet write{ .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
	write.dstSet = set;
	write.dstBinding = binding;
	write.dstArrayElement = 0;
	write.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	write.descriptorCount = 1;
	write.pBufferInfo = &bufferInfo;

	vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
}
