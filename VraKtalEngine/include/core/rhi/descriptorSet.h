#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include <core/rhi/image.h>

namespace core::rhi
{
	class Buffer;

	enum class DescriptorType
	{
		UniformBuffer, 
		StorageBuffer,
		ImageSampler
	};

	enum class ShaderStage
	{
		Vertex,
		Fragment,
		Compute
	};

	struct DescriptorBinding
	{
		uint32_t binding;
		DescriptorType type;
		ShaderStage stage;
	};

	struct DescriptorSetLayoutDesc
	{
		std::vector<DescriptorBinding> bindings;
	};

	class DescriptorSetLayout
	{
	public:
		explicit DescriptorSetLayout(void* device, const DescriptorSetLayoutDesc& desc);
		~DescriptorSetLayout();

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;

		friend class DescriptorSet;
	};

	class DescriptorSet
	{
	public:
		explicit DescriptorSet(void* device, void* pool, const DescriptorSetLayoutDesc& layoutDesc);
		~DescriptorSet();

		void BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range);
		void BindImage(uint32_t binding, Image* image, const ImageUsage& usage);

	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};
}

#endif //VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
