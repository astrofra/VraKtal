#ifndef VRAKTAL_CORE_RHI_TEXTURE_H
#define VRAKTAL_CORE_RHI_TEXTURE_H
#pragma once

#include <cstdint>
#include <core/rhi/enums.h>

namespace core::rhi
{
	struct TextureDesc
	{
		uint32_t width;
		uint32_t height;
		Format format;
		TextureUsage usage;
		uint32_t mipLevels;
		uint32_t layers;
	};

	class Texture
	{
		~Texture();

		void UploadData(const void* pixels, size_t size, uint32_t mipLevel = 0);
		void GenerateMipmaps(class CommandBuffer* cmd);
		void TransitionLayout(class CommandBuffer* cmd, TextureLayout newLayout);
	};
}

#endif //VRAKTAL_CORE_RHI_TEXTURE_H
