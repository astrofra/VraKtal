#ifndef VRAKTAL_CORE_RHI_ENUMS_H
#define VRAKTAL_CORE_RHI_ENUMS_H
#pragma once

#include <cstdint>

namespace core::rhi
{
	enum class Format : uint32_t
	{
		Undefined = 0,

        R8_UNorm,
        RG8_UNorm,
        RGB8_UNorm,
        RGBA8_UNorm,
        BGRA8_UNorm,
        RGBA8_SRGB,

        R16_Float,
        RG16_Float,
        RGBA16_Float,
        R32_Float,
        RG32_Float,
        RGBA32_Float,

        D16_UNorm,
        D24_UNorm_S8_UInt,
        D32_Float,
        D32_Float_S8_UInt,
	};

    enum class TextureUsage : uint32_t
    {
        None                    = 0,
        TransferSrc             = 1 << 0,
        TransferDst             = 1 << 1,
        Sampled                 = 1 << 2,
        Storage                 = 1 << 3,
        ColorAttachment         = 1 << 4,
        DepthStencilAttachment  = 1 << 5,
        InputAttachment         = 1 << 6,
    };


    inline TextureUsage operator|(TextureUsage a, TextureUsage b)
    {
        return static_cast<TextureUsage>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline bool operator&(TextureUsage a, TextureUsage b)
    {
        return (static_cast<uint32_t>(a) & static_cast<uint32_t>(b)) != 0;
    }

    enum class TextureLayout : uint8_t
    {
        Undefined,
        General,
        ColorAttachment,
        DepthStencilAttachment,
        ShaderReadOnly,
        TransferSrc,
        TransferDst,
        PresentSrc
    };

    enum class BufferUsage
    {
        Vertex,
        Index,
        Uniform,
        Staging,
        Storage,
        TransferDst,
        TransferSrc
    };

    enum class MemoryUsage
    {
        CPU,
        GPU
    };

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

    enum class GpuImageUsage
    {
        ColorAttachment,
        DepthStencilAttachment,
        Sampled,
        Storage
    };

    enum class LoadOp 
    {
        Clear, 
        Load, 
        DontCare 
    };

    enum class StoreOp 
    { 
        Store,
        DontCare 
    };

}

#endif //VRAKTAL_CORE_RHI_ENUMS_H
