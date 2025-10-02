#ifndef VRAKTAL_CORE_RHI_GPU_DEVICE_VK_H
#define VRAKTAL_CORE_RHI_GPU_DEVICE_VK_H
#pragma once

#include <core/rhi/gpuDevice.h>
#include <core/rhi/gpuImage.h>

#include <vulkan/vulkan.h>
#include "../src/vkb/VkBootstrap.h"
#include <vma/vk_mem_alloc.h>

#include <vector>

namespace core::rhi
{
    struct GpuDevice::Impl
    {
        explicit Impl(const Window& window);
        ~Impl();

        CommandBuffer* CreateCommandBuffer();
        void DestroyCommandBuffer(CommandBuffer* cmd);

        void RecreateSwapchain();
        void WrapSwapchainImages();
        void DeleteWrappedImages();

        bool BeginFrame(uint32_t& imageIndex);
        void EndFrame(uint32_t imageIndex, VkCommandBuffer cmd);

        void UploadToBuffer(VkBuffer dst, const void* data, VkDeviceSize size);
        void CreateDepthBuffer();
        void DestroyDepthBuffer();
        VkFormat FindDepthFormat();
        void WaitIdle();

        void CreateInstance();
        void CreateSurface(const Window& window);
        void PickPhysicalDevice();
        void CreateLogicalDevice();
        void CreateAllocator();
        void CreateSwapchain(uint32_t width, uint32_t height);
        void DestroySwapchain();
        void CreateCommandPool();
        void DestroyCommandPool();
        void CreateSyncObjects();
        void DestroySyncObjects();

        VmaAllocator    Allocator    (){return m_allocator;     }
        VkQueue         GraphicsQueue(){return m_graphicsQueue; }
        VkDevice        Device       (){return m_device;        }
        VkCommandPool   CommandPool  (){return m_cmdPool;       }
        VkInstance      Instance     (){ return m_instance; }
        GpuImage* GetSwapchainImage(uint32_t index) const;

        vkb::Instance m_instance;
        VkSurfaceKHR m_surface = VK_NULL_HANDLE;
        vkb::PhysicalDevice m_physicalDevice;
        VkDevice m_device = VK_NULL_HANDLE;

        VkQueue m_graphicsQueue = VK_NULL_HANDLE;
        uint32_t m_graphicsQueueFamily = 0;

        VmaAllocator m_allocator = VK_NULL_HANDLE;

        VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
        VkFormat m_swapFormat = VK_FORMAT_B8G8R8A8_UNORM;
        VkExtent2D m_swapExtent = {};
        std::vector<VkImage> m_swapImages;
        std::vector<VkImageView> m_swapImageViews;
        std::vector<GpuImage*> m_swapchainImageWrappers;

        VkCommandPool m_cmdPool = VK_NULL_HANDLE;

        struct FrameSync
        {
            VkSemaphore imageAvailable = VK_NULL_HANDLE;
            VkSemaphore renderFinished = VK_NULL_HANDLE;
            VkFence inFlight = VK_NULL_HANDLE;
        };

        static constexpr int OVERLAPPED_FRAMES = 2;
        std::vector<FrameSync> m_frames;
        uint32_t m_currentFrame = 0;

        VkImage m_depthImage = VK_NULL_HANDLE;
        VmaAllocation m_depthAllocation = VK_NULL_HANDLE;
        VkImageView m_depthImageView = VK_NULL_HANDLE;
        VkFormat m_depthFormat = VK_FORMAT_D32_SFLOAT;
    };
}

#endif // VRAKTAL_CORE_RHI_GPU_DEVICE_VK_H
