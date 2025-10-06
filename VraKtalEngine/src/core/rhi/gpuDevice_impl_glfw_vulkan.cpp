#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"
#include "../src/core/rhi/commandBuffer_impl_vulkan.h"
#include "../src/core/rhi/gpuImage_impl_vulkan.h"
#include "../src/core/gpu_details/initializer_vulkan.h"

#include <core/window.h>

#include "../src/vkb/VkBootstrap.h"
#include <GLFW/glfw3.h>

#define VMA_IMPLEMENTATION
#include <vma/vk_mem_alloc.h>

#pragma comment(lib, "vulkan-1.lib")

#include <stdexcept>
#include <array>
#include <limits>
#include <iostream>

using namespace core::rhi;

GpuDevice::GpuDevice(Window& window)
{
    m_impl = std::make_unique<Impl>(window);
    m_impl->WrapSwapchainImages();
}

GpuDevice::~GpuDevice() 
{
    
}

GpuDevice::Impl& GpuDevice::GetImpl() 
{
    return *m_impl;
}

bool core::rhi::GpuDevice::BeginFrame(uint32_t& imageIndex)
{
   return m_impl->BeginFrame(imageIndex);
}

void core::rhi::GpuDevice::EndFrame(uint32_t imageIndex, CommandBuffer& cmd)
{
    m_impl->EndFrame(imageIndex, cmd.GetImpl().GetNative());
}

std::pair<int, int> GpuDevice::GetSize() 
{
    return std::pair<int, int>(m_impl->m_swapExtent.width, m_impl->m_swapExtent.height);
};

GpuImage* GpuDevice::GetSwapchainImage(uint32_t index) const 
{
    return m_impl->GetSwapchainImage(index);
};

void GpuDevice::WaitIdle()
{
    m_impl->WaitIdle();
}

void* GpuDevice::GetInstance() const
{
    return m_impl->Instance();
}

GpuDevice::Impl::Impl(const Window& window)
{
    CreateInstance();
    CreateSurface(window);
    PickPhysicalDevice();
    CreateLogicalDevice();
    CreateAllocator();
    CreateSwapchain(window.Size().first, window.Size().second);
    CreateDepthBuffer();
    CreateCommandPool();
    CreateSyncObjects();
}

GpuDevice::Impl::~Impl()
{
    WaitIdle();
    DestroySyncObjects();
    DeleteWrappedImages();
    DestroyCommandPool();
    DestroyDepthBuffer();
    DestroySwapchain();

    if (m_allocator) vmaDestroyAllocator(m_allocator);
    if (m_device) vkDestroyDevice(m_device, nullptr);
    if (m_surface) vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
    if (m_instance) vkDestroyInstance(m_instance, nullptr);
}

void GpuDevice::Impl::CreateSurface(const Window& window)
{
    if (glfwCreateWindowSurface(m_instance, window.GlfwHandle(), nullptr, &m_surface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface");
    }
}

void GpuDevice::Impl::WaitIdle()
{
    vkDeviceWaitIdle(m_device);
}


CommandBuffer* GpuDevice::Impl::CreateCommandBuffer()
{
    return new CommandBuffer(*reinterpret_cast<GpuDevice*>(this));
}

void GpuDevice::Impl::DestroyCommandBuffer(CommandBuffer* commandBuffer)
{
    delete commandBuffer;
}

void GpuDevice::Impl::RecreateSwapchain()
{
    WaitIdle();
    DeleteWrappedImages();
    DestroySwapchain();
    CreateSwapchain(m_swapExtent.width, m_swapExtent.height);
    WrapSwapchainImages();
}

void GpuDevice::Impl::CreateInstance()
{
    vkb::InstanceBuilder builder;
    auto instance = builder
        .set_app_name("Vulkan Window")
        .request_validation_layers(true)
        .use_default_debug_messenger()
        .require_api_version(1, 3, 0)
        .build();

    if (!instance)
    {
        throw std::runtime_error("failed to create instance");
    }

    m_instance = instance.value();
}

void GpuDevice::Impl::PickPhysicalDevice()
{
    VkPhysicalDeviceVulkan13Features f13 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES};
    f13.dynamicRendering = VK_TRUE;
    f13.synchronization2 = VK_TRUE;

    vkb::PhysicalDeviceSelector selector { m_instance, m_surface };
    auto physical_device = selector
        .set_surface(m_surface)
        .set_minimum_version(1,3)
        .set_required_features_13(f13)
        .prefer_gpu_device_type(vkb::PreferredDeviceType::discrete)
        .allow_any_gpu_device_type(false)
        .select();

    if (!physical_device)
    {
        throw std::runtime_error("Physical device selection failed");
    }

    m_physicalDevice = physical_device.value();
}

void GpuDevice::Impl::CreateLogicalDevice()
{
    vkb::DeviceBuilder builder{ m_physicalDevice };
    auto device = builder.build();

    if (!device)
    {
        throw std::runtime_error("Logical device creation failed");
    }

    m_device = device.value();
    m_graphicsQueue = device.value().get_queue(vkb::QueueType::graphics).value();
    m_graphicsQueueFamily = device.value().get_queue_index(vkb::QueueType::graphics).value();
}

void GpuDevice::Impl::CreateAllocator()
{
    VmaAllocatorCreateInfo alloc_info{};
    alloc_info.instance = m_instance;
    alloc_info.physicalDevice = m_physicalDevice;
    alloc_info.device = m_device;
   // alloc_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;

    if (vmaCreateAllocator(&alloc_info, &m_allocator) != VK_SUCCESS)
    {
        throw std::runtime_error("VMA allocator creation failed");
    }
}

void GpuDevice::Impl::CreateSwapchain(uint32_t _width, uint32_t _height)
{
    vkb::SwapchainBuilder swapchainBuilder { m_physicalDevice, m_device, m_surface };
    auto swapchain = swapchainBuilder
        .set_desired_extent(_width, _height)
        .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
        .set_desired_format({ m_swapFormat, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
        .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
        .build();

    if (!swapchain)
    {
        throw std::runtime_error("Swapchain creation failed");
    }

    auto value = swapchain.value();
    m_swapchain = value.swapchain;
    m_swapExtent = value.extent;
    m_swapImages = value.get_images().value();
    m_swapImageViews = value.get_image_views().value();
    m_swapFormat = value.image_format;
}

void GpuDevice::Impl::DestroySwapchain()
{
    for (auto v : m_swapImageViews)
    {
        vkDestroyImageView(m_device, v, nullptr);
    }

    m_swapImageViews.clear();
    m_swapImages.clear();

    if (m_swapchain)
    {
        vkDestroySwapchainKHR(m_device, m_swapchain, nullptr);
        m_swapchain = VK_NULL_HANDLE;
    }
}

void GpuDevice::Impl::CreateCommandPool()
{
    VkCommandPoolCreateInfo commandPool_info = gpu_details::CommandPoolCreateInfo(m_graphicsQueueFamily);


    if (vkCreateCommandPool(m_device, &commandPool_info, nullptr, &m_cmdPool) != VK_SUCCESS)
    {
        throw std::runtime_error("Command pool creation failed");
    }
}

void GpuDevice::Impl::DestroyCommandPool()
{
    if (m_cmdPool)
    {
        vkDestroyCommandPool(m_device, m_cmdPool, nullptr);
        m_cmdPool = VK_NULL_HANDLE;
    }
}

void GpuDevice::Impl::CreateSyncObjects()
{
    m_frames.resize(OVERLAPPED_FRAMES);
    VkSemaphoreCreateInfo semInfo = gpu_details::SemaphoreCreateInfo();
    VkFenceCreateInfo fenceInfo = gpu_details::FenceCreateInfo(true);

    for (int i = 0; i < OVERLAPPED_FRAMES; ++i)
    {
        if (vkCreateSemaphore(m_device, &semInfo, nullptr, &m_frames[i].imageAvailable) != VK_SUCCESS ||
            vkCreateSemaphore(m_device, &semInfo, nullptr, &m_frames[i].renderFinished) != VK_SUCCESS ||
            vkCreateFence(m_device, &fenceInfo, nullptr, &m_frames[i].inFlight) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create sync objects");
        }
    }
}

void GpuDevice::Impl::DestroySyncObjects()
{
    for (auto& f : m_frames)
    {
        if (f.imageAvailable) vkDestroySemaphore(m_device, f.imageAvailable, nullptr);
        if (f.renderFinished) vkDestroySemaphore(m_device, f.renderFinished, nullptr);
        if (f.inFlight) vkDestroyFence(m_device, f.inFlight, nullptr);
    }
    m_frames.clear();
}

void GpuDevice::Impl::WrapSwapchainImages()
{
    DeleteWrappedImages();
    m_swapchainImageWrappers.reserve(m_swapImages.size());
    for (size_t i = 0; i < m_swapImages.size(); ++i)
    {
        auto impl = std::make_unique<GpuImage::Impl>(
            m_device, m_swapImages[i], m_swapImageViews[i], m_swapFormat,
            m_swapExtent.width, m_swapExtent.height
        );

        auto* wrapper = new GpuImage(std::move(impl));
        m_swapchainImageWrappers.push_back(wrapper);
    }
}

void GpuDevice::Impl::DeleteWrappedImages()
{
    for (auto* img : m_swapchainImageWrappers)
    {
        delete img;
    }
    m_swapchainImageWrappers.clear();
}

bool GpuDevice::Impl::BeginFrame(uint32_t& imageIndex)
{
    FrameSync& sync = m_frames[m_currentFrame];

    vkWaitForFences(m_device, 1, &sync.inFlight, VK_TRUE, UINT64_MAX);

    VkResult res = vkAcquireNextImageKHR(m_device, m_swapchain, UINT64_MAX, sync.imageAvailable, VK_NULL_HANDLE, &imageIndex);

    if (res == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapchain();
        return false;
    }
    else if (res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("Failed to acquire swapchain image");
    }

    vkResetFences(m_device, 1, &sync.inFlight);
    return true;
}

void GpuDevice::Impl::EndFrame(uint32_t imageIndex, VkCommandBuffer cmd)
{
    FrameSync& sync = m_frames[m_currentFrame];

    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkSubmitInfo submit = gpu_details::SubmitInfo(
        cmd,
        sync.imageAvailable,
        waitStage,
        sync.renderFinished
    );

    if (vkQueueSubmit(m_graphicsQueue, 1, &submit, sync.inFlight) != VK_SUCCESS)
        throw std::runtime_error("Queue submit failed");

    VkPresentInfoKHR present = gpu_details::PresentInfo(sync.renderFinished, m_swapchain, imageIndex);
    VkResult res = vkQueuePresentKHR(m_graphicsQueue, &present);

    if (res == VK_ERROR_OUT_OF_DATE_KHR || res == VK_SUBOPTIMAL_KHR)
    {
        RecreateSwapchain();
    }
    else if (res != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to present swapchain image");
    }

    m_currentFrame = (m_currentFrame + 1) % OVERLAPPED_FRAMES;
}

GpuImage* GpuDevice::Impl::GetSwapchainImage(uint32_t index) const
{
    //std::cout << m_swapchainImageWrappers.size() << std::endl;
    return m_swapchainImageWrappers[index];
}


void GpuDevice::Impl::UploadToBuffer(VkBuffer dst, const void* data, VkDeviceSize size)
{
    VkBuffer stagingBuffer;
    VmaAllocation stagingAlloc;

    VkBufferCreateInfo bufferInfo{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    if (vmaCreateBuffer(m_allocator, &bufferInfo, &allocInfo, &stagingBuffer, &stagingAlloc, VK_NULL_HANDLE) != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create buffer");
    }

    void* mapped;
    vmaMapMemory(m_allocator, stagingAlloc, &mapped);
    memcpy(mapped, data, static_cast<size_t>(size));
    vmaUnmapMemory(m_allocator, stagingAlloc);

    VkCommandBufferAllocateInfo allocInfoCmd{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};
    allocInfoCmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfoCmd.commandBufferCount = 1;
    allocInfoCmd.commandPool = m_cmdPool;

    VkCommandBuffer cmd;
    vkAllocateCommandBuffers(m_device, &allocInfoCmd, &cmd);

    VkCommandBufferBeginInfo beginInfo{VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO};
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(cmd, stagingBuffer, dst, 1, &copyRegion);

    vkEndCommandBuffer(cmd);

    VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd;

    vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(m_graphicsQueue);

    vkFreeCommandBuffers(m_device,  m_cmdPool, 1, &cmd);

    vmaDestroyBuffer(m_allocator, stagingBuffer, stagingAlloc);
}

VkFormat GpuDevice::Impl::FindDepthFormat()
{
    std::vector<VkFormat> candidates = {
        VK_FORMAT_D32_SFLOAT,
        VK_FORMAT_D32_SFLOAT_S8_UINT,
        VK_FORMAT_D24_UNORM_S8_UINT
    };

    for (VkFormat format : candidates) 
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(m_physicalDevice, format, &props);

        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) 
        {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported depth format");
}

void GpuDevice::Impl::CreateDepthBuffer()
{
    m_depthFormat = FindDepthFormat();

    VkImageCreateInfo imageInfo = gpu_details::ImageCreateInfo(
        m_swapExtent.width,
        m_swapExtent.height,
        m_depthFormat,
        VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    VmaAllocationCreateInfo allocInfo{};
    allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

    if (vmaCreateImage(m_allocator, &imageInfo, &allocInfo, &m_depthImage, &m_depthAllocation, nullptr) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create depth image");
    }

    VkImageViewCreateInfo viewInfo = gpu_details::ImageViewCreateInfo(m_depthImage, m_depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);

    if (vkCreateImageView(m_device, &viewInfo, nullptr, &m_depthImageView) != VK_SUCCESS) 
    {
        throw std::runtime_error("Failed to create depth image view");
    }
}

void GpuDevice::Impl::DestroyDepthBuffer()
{
    if (m_depthImageView) 
    {
        vkDestroyImageView(m_device, m_depthImageView, nullptr);
        m_depthImageView = VK_NULL_HANDLE;
    }
    if (m_depthImage) 
    {
        vmaDestroyImage(m_allocator, m_depthImage, m_depthAllocation);
        m_depthImage = VK_NULL_HANDLE;
    }
}