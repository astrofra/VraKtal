#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_POOL_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_POOL_H
#pragma once

#include <memory>

namespace core::rhi
{
	class GpuDevice;

	class DescriptorPool
	{	
	public:
		explicit DescriptorPool(GpuDevice& device);
		~DescriptorPool() noexcept;

		struct Impl;
		Impl& GetImpl();
	private:
		std::unique_ptr<Impl> m_impl;
	};
}

#endif //VRAKTAL_CORE_RHI_DESCRIPTOR_POOL_H
