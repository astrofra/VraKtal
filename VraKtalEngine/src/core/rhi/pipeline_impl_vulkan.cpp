#include "../src/core/rhi/pipeline_impl_vulkan.h"
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"

using namespace core::rhi;

Pipeline::Pipeline(GpuDevice& device, void* nativePipeline, void* nativeLayout)
{
    VkPipeline vkPipeline = reinterpret_cast<VkPipeline>(nativePipeline);
    VkPipelineLayout vkLayout = reinterpret_cast<VkPipelineLayout>(nativeLayout);

    m_impl = std::make_unique<Impl>(device, vkPipeline, vkLayout);
}

Pipeline::~Pipeline() = default;

Pipeline::Impl::Impl(GpuDevice& device, VkPipeline pipeline, VkPipelineLayout layout)
    : m_device(device), m_pipeline(pipeline), m_layout(layout)
{
}

Pipeline::Impl::~Impl()
{
    if (m_pipeline)
    {
        vkDestroyPipeline(m_device.GetImpl().m_device, m_pipeline, nullptr);
    }
    if (m_layout)
    {
        vkDestroyPipelineLayout(m_device.GetImpl().m_device, m_layout, nullptr);
    }
}

Pipeline::Impl& Pipeline::GetImpl() { return *m_impl; }
