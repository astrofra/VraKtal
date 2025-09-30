#ifndef VRAKTAL_CORE_RHI_GPUDEVICE_H
#define VRAKTAL_CORE_RHI_GPUDEVICE_H
#pragma once

#include <cstdint>

namespace core
{
    class Window;

    namespace rhi
    {
        class CommandBuffer;

        class GpuDevice
        {
            struct Impl;
            Impl* m_impl = nullptr;

        public:
            explicit GpuDevice(Window& window);
            ~GpuDevice();

            CommandBuffer* CreateCommandBuffer();
            void DestroyCommandBuffer(CommandBuffer* commandBuffer);

            void RecreateSwapchain();

            Impl& GetImpl();
        };
    }
}

#endif // VRAKTAL_CORE_RHI_GPUDEVICE_H
