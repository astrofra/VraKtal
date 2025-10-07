#ifndef VRAKTAL_CORE_RHI_SWAPCHAIN_H
#define VRAKTAL_CORE_RHI_SWAPCHAIN_H
#pragma once

#include <cstdint>

namespace core::rhi
{
	class Swapchain
	{
	public:
		Swapchain();
		~Swapchain();

		void Resize(uint32_t width, uint32_t height);
		void Present();

		uint32_t GetCurrentFrameIndex() const;
		uint32_t GetImageCount() const;
	};
}

#endif //VRAKTAL_CORE_RHI_SWAPCHAIN_H
