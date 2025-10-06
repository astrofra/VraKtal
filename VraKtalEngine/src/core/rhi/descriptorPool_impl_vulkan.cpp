#include "../src/core/rhi/descriptorPool_impl_vulkan.h"
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"

#include <vector>

using namespace core::rhi;

DescriptorPool::DescriptorPool(GpuDevice& device)
{
	m_impl = std::make_unique<Impl>(device);
}

DescriptorPool::~DescriptorPool() = default;
DescriptorPool::Impl& DescriptorPool::GetImpl() { return *m_impl; }

DescriptorPool::Impl::Impl(GpuDevice& device)
	: m_device(device)
{
	std::vector<VkDescriptorPoolSize> poolSizes =
	{
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1024 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1024 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 2048 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1024 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,           512 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,    256 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,    256 },
		{ VK_DESCRIPTOR_TYPE_SAMPLER,                 512 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,        256 }
	};

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = 2048;
	poolInfo.poolSizeCount = uint32_t(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();

	vkCreateDescriptorPool(device.GetImpl().m_device, &poolInfo, nullptr, &pool);
}

DescriptorPool::Impl::~Impl()
{
	vkDestroyDescriptorPool(m_device.GetImpl().m_device, pool, nullptr);
}
