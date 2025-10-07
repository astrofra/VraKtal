#ifndef VRAKTAL_CORE_RHI_GPU_IMAGE_H
#define VRAKTAL_CORE_RHI_GPU_IMAGE_H
#pragma once

#include <memory>
#include <cstdint>

#include <core/rhi/enums.h>

namespace core::rhi
{
    struct GpuImageDesc
    {
        uint32_t width = 0;
        uint32_t height = 0;
        GpuImageUsage usage = GpuImageUsage::ColorAttachment;
    };

    class GpuDevice;

    class GpuImage
    {
    public:
        struct Impl;

        explicit GpuImage(GpuDevice& device, const GpuImageDesc& desc);
        explicit GpuImage(std::unique_ptr<Impl> impl);
        ~GpuImage();

        uint32_t Width() const;
        uint32_t Height() const;

        void GetDescriptorInfo(void* outInfo, void* sampler = nullptr) const;

        Impl& GetImpl();

    private:
        std::unique_ptr<Impl> m_impl;
    };
}

#endif // VRAKTAL_CORE_RHI_GPU_IMAGE_H
