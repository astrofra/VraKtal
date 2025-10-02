#ifndef VRAKTAL_CORE_RHI_GPUDEVICE_H
#define VRAKTAL_CORE_RHI_GPUDEVICE_H
#pragma once

#include <cstdint>
#include <memory>



namespace core
{
    class Window;

    namespace rhi
    {
        class CommandBuffer;
        class GpuImage;

        class GpuDevice
        {
            struct Impl;
            std::unique_ptr<Impl> m_impl;

        public:
            explicit GpuDevice(Window& window);
            ~GpuDevice();

            bool BeginFrame(uint32_t& imageIndex);
            void EndFrame(uint32_t imageIndex, CommandBuffer& cmd);

            CommandBuffer* CreateCommandBuffer();
            void DestroyCommandBuffer(CommandBuffer* commandBuffer);
            void RecreateSwapchain();

            void WaitIdle();

            std::pair<int, int> GetSize(); //retrurn SwapExtent for vulk

            GpuImage* GetSwapchainImage(uint32_t index) const;
            void* GetInstance() const;
            
            Impl& GetImpl();
        };
    }
}

#endif // VRAKTAL_CORE_RHI_GPUDEVICE_H
