#ifndef VRAKTAL_CORE_RHI_COMMAND_BUFFER_H
#define VRAKTAL_CORE_RHI_COMMAND_BUFFER_H
#pragma once

#include <memory>

namespace core::rhi
{
    struct RenderingInfo;

    class Pipeline;
    class GpuDevice;

    class CommandBuffer
    {
    public:
        CommandBuffer(GpuDevice& device);
        ~CommandBuffer();

        void Begin();
        void End();

        void BeginRendering(const RenderingInfo& info, uint32_t imageIndex);
        void EndRendering(uint32_t imageIndex);

        void BindPipeline(Pipeline* pipeline);
        void Draw(uint32_t vertexCount, uint32_t width, uint32_t height);

        struct Impl;
        Impl& GetImpl();
    private:
        std::unique_ptr<Impl> m_impl;
    };
}

#endif // VRAKTAL_CORE_RHI_COMMAND_BUFFER_H
