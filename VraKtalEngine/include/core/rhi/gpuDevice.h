#ifndef VRAKTAL_CORE_RHI_GPUDEVICE_H
#define VRAKTAL_CORE_RHI_GPUDEVICE_H
#pragma once

#include <cstdint>
#include <memory>

#include <core/rhi/enums.h>

namespace core
{
    class Window;

    namespace rhi
    {
        class Swapchain;
        class CommandBuffer;
        class Shader;
        class Texture;
        class Buffer;
        class RenderPass;
        class GpuImage;

        struct ShaderDesc;

        class GpuDevice
        {
            struct Impl;
            std::unique_ptr<Impl> m_impl;

        public:
            explicit GpuDevice(Window& window);
            ~GpuDevice();

            bool BeginFrame(uint32_t& imageIndex);
            void EndFrame(uint32_t imageIndex, CommandBuffer& cmd);

            Swapchain       CreateSwapchain(void* windowHandle, uint32_t width, uint32_t height);
            CommandBuffer   CreateCommandBuffer();
            Buffer          CreateBuffer(size_t size, BufferUsage usage, MemoryUsage memUsage);
            Texture         CreateTexture(uint32_t width, uint32_t height, Format format, TextureUsage usage);
            Shader          CreateShader(const ShaderDesc& desc);

            void WaitIdle();

            std::pair<int, int> GetSize(); //retrurn SwapExtent for vulk

            GpuImage* GetSwapchainImage(uint32_t index) const;
            void* GetInstance() const;
            
            Impl& GetImpl();

            void BeginFrame();
            void EndFrame();
        };
    }
}

#endif // VRAKTAL_CORE_RHI_GPUDEVICE_H
