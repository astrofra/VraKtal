#ifndef VRAKTAL_CORE_RHI_GPUDEVICE_H
#define VRAKTAL_CORE_RHI_GPUDEVICE_H
#pragma once

#include <cstdint>

namespace core::rhi
{
    class CommandBuffer;
    class Window;

    class GpuDevice
    {
        struct Impl;
        Impl* m_impl = nullptr;
    public:
        GpuDevice(Window& window);
        ~GpuDevice();

        CommandBuffer* CreateCommandBuffer();
        void DestroyCommandBuffer(CommandBuffer* _commandBuffer);

        void RecreateSwapchain();

        Impl& GetImpl();
    };
}

#endif // VRAKTAL_CORE_RHI_GPUDEVICE_H
