#ifndef VRAKTAL_CORE_RHI_SWAPCHAIN_H
#define VRAKTAL_CORE_RHI_SWAPCHAIN_H
#pragma once

#include <cstdint>
#include <memory>

namespace core::rhi
{
	class Swapchain
	{
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	public:
		Swapchain();
		~Swapchain();

		void Resize(uint32_t width, uint32_t height);
		void Present();

		uint32_t GetCurrentFrameIndex() const;
		uint32_t GetImageCount() const;

		Impl& GetImpl();
	};
}

#endif //VRAKTAL_CORE_RHI_SWAPCHAIN_H
