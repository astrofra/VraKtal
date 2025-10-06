#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_POOL_VK_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_POOL_VK_H
#pragma once

#include <core/rhi/descriptorPool.h>
#include <core/rhi/gpuDevice.h>
#include <vulkan/vulkan.h>

namespace core::rhi
{
	struct DescriptorPool::Impl
	{
		Impl(GpuDevice& device);
		~Impl();

		VkDescriptorPool pool = VK_NULL_HANDLE;

	private:
		GpuDevice& m_device;
	};
}

#endif //VRAKTAL_CORE_RHI_DESCRIPTOR_POOL_VK_H