#ifndef VRAKTAL_CORE_RHI_TEXTURE_VK_H
#define VRAKTAL_CORE_RHI_TEXTURE_VK_H
#pragma once

#include <core/rhi/texture.h>
#include <core/rhi/gpuDevice.h>

#include <vulkan/vulkan.h>
#include <memory>

namespace core::rhi
{
	struct Texture::Impl
	{
	public:
		Impl(GpuDevice* device, TextureDesc& desc);
		~Impl();

		void UploadData(const void* data, size_t size, uint32_t mipLevel = 0);
		void TransitionLayout(rhi::CommandBuffer* cmd, rhi::TextureLayout newLayout);
		void GenerateMipmaps(rhi::CommandBuffer* cmd);
		
	private:
		GpuDevice* m_device;
		TextureDesc m_desc;

		VkImage m_image			= VK_NULL_HANDLE;
		VkDeviceMemory m_memory = VK_NULL_HANDLE;
		VkImageView m_view		= VK_NULL_HANDLE;

		void CreateImageAndView();
		void DestroyImageAndView();
	};
}

#endif //VRAKTAL_CORE_RHI_TEXTURE_VK_H
