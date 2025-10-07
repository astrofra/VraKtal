#include "../src/core/rhi/shader_impl_vulkan.h"
#include "../src/core/rhi/gpuDevice_impl_glfw_vulkan.h"
#include "../gpu_details/converter_vulkan.h"

using namespace core::rhi;

Shader::Shader(GpuDevice* device, ShaderDesc& desc)
{
    m_impl = std::make_unique<Impl>(device, desc);
}

Shader::~Shader() = default;
Shader::Impl& Shader::GetImpl() { return *m_impl; }

Shader::Impl::Impl(GpuDevice* device, ShaderDesc& desc)
	: m_device(device)
{
	m_modules.reserve(desc.stages.size());
	m_stages.reserve(desc.stages.size());

    for (const auto& s : desc.stages)
    {
        if (s.spirv.empty()) continue;

        VkShaderModuleCreateInfo moduleInfo{};
        moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleInfo.codeSize = s.spirv.size();
        moduleInfo.pCode = reinterpret_cast<const uint32_t*>(s.spirv.data());

        VkShaderModule module = VK_NULL_HANDLE;
        if (vkCreateShaderModule(m_device->GetImpl().m_device, &moduleInfo, nullptr, &module) != VK_SUCCESS) 
        {
            throw std::runtime_error("Failed to create shader module");
        }
        m_modules.push_back(module);

        VkPipelineShaderStageCreateInfo stageInfo{};
        stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageInfo.stage = gpu_details::ToVkShaderStage(s.stage);
        stageInfo.module = module;
        stageInfo.pName = s.entryPoint;
        m_stages.push_back(stageInfo);
    }
}

void Shader::Impl::Bind(CommandBuffer* cmd)
{

}

Shader::Impl::~Impl()
{
    for (auto m : m_modules) 
    {
        if (m != VK_NULL_HANDLE) vkDestroyShaderModule(m_device->GetImpl().m_device, m, nullptr);
    }
}
