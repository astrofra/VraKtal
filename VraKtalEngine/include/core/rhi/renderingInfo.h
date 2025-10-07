#ifndef VRAKTAL_CORE_RHI_RENDERING_INFO_H
#define VRAKTAL_CORE_RHI_RENDERING_INFO_H
#pragma once

#include <cstdint>
#include <vector>

#include <core/rhi/enums.h>

namespace core::rhi
{
    struct ClearColor
    {
        float r, g, b, a;
    };

    class GpuImage;

    struct RenderingAttachment
    {
        GpuImage* image;
        ClearColor clearValue;
        LoadOp loadOp;
        StoreOp storeOp;
    };

    struct RenderingInfo
    {
        std::vector<RenderingAttachment> colorAttachments;
        uint32_t width = 0;
        uint32_t height = 0;
    };
}

#endif //VRAKTAL_CORE_RHI_RENDERING_INFO_H