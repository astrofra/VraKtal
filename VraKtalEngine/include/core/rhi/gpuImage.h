#ifndef VRAKTAL_CORE_RHI_GPU_IMAGE_H
#define VRAKTAL_CORE_RHI_GPU_IMAGE_H
#pragma once

#include <memory>
#include <cstdint>

namespace core::rhi
{
    enum class GpuImageUsage
    {
        ColorAttachment,
        DepthStencilAttachment,
        Sampled,
        Storage
    };

    struct GpuImageDesc
    {
        uint32_t width = 0;
        uint32_t height = 0;
        GpuImageUsage usage = GpuImageUsage::ColorAttachment;
    };

    class GpuImage
    {
    public:
        explicit GpuImage(const GpuImageDesc& desc);
        ~GpuImage();

        uint32_t Width() const;
        uint32_t Height() const;

        void GetDescriptorInfo(void* outInfo) const;

    private:
        struct Impl;
        std::unique_ptr<Impl> m_impl;
    };
}

#endif // VRAKTAL_CORE_RHI_GPU_IMAGE_H
