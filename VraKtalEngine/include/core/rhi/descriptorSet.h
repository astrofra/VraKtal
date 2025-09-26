#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
#pragma once

#include <cstdint>
#include <vector>

#include <core/rhi/buffer.h>
#include <core/rhi/image.h>

namespace core::rhi
{
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

	class DescriptorSet
	{
	public:
		virtual ~DescriptorSet() = default;

		virtual void BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range) = 0;
		virtual void BindImage(uint32_t binding, Image* image, const ImageUsage& usage) = 0;
	};
}

#endif //VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
