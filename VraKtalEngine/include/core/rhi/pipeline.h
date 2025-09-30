#ifndef VRAKTAL_CORE_RHI_PIPELINE_H
#define VRAKTAL_CORE_RHI_PIPELINE_H
#pragma once

#include <memory>

namespace core::rhi
{
    class Pipeline
    {
    public:
        Pipeline(GpuDevice& device, void* nativePipeline, void* nativeLayout);
        ~Pipeline();

        struct Impl;
        Impl& GetImpl();

    protected:
        std::unique_ptr<Impl> m_impl;
    };
}

#endif // VRAKTAL_CORE_RHI_PIPELINE_H
