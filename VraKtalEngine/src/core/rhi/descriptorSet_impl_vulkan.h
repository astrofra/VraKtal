#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
#pragma once

#include <vulkan/vulkan.h>

#include <core/rhi/descriptorSet.h>
#include <core/rhi/buffer.h>
#include <core/rhi/image.h>

namespace core::rhi::vulkan
{
	VkDescriptorType ToVkDescriptorType(DescriptorType type);
	VkShaderStageFlags ToVkShaderStage(ShaderStage stage);

	class DescriptorSetLayoutVulkan
	{
	public:
		DescriptorSetLayoutVulkan(VkDevice device, const DescriptorSetLayoutDesc& desc);
		~DescriptorSetLayoutVulkan();

		VkDescriptorSetLayout GetHandle() const { return m_layout; }

	private:
		VkDevice m_device;
		VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
	};

	class DescriptorSetVulkan : public DescriptorSet
	{
	public:
		DescriptorSetVulkan(VkDevice device, VkDescriptorPool pool, DescriptorSetLayoutVulkan* layout);
		~DescriptorSetVulkan() override;

		void BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range) override;
		void BindImage(uint32_t binding, Image* image, const ImageUsage& usage) override;
		
		VkDescriptorSet GetHandle() const { return m_set; }
	private:
		VkDevice m_device;
		VkDescriptorSet m_set = VK_NULL_HANDLE;
		DescriptorSetLayoutVulkan* m_layout = nullptr;
	};
}

#endif //VRAKTAL_CORE_RHI_DESCRIPTOR_SET_VK_H
