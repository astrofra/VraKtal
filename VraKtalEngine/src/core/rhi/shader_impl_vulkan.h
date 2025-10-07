#ifndef VRAKTAL_CORE_RHI_SHADER_VK_H
#define VRAKTAL_CORE_RHI_SHADER_VK_H
#pragma once

#include <core/rhi/shader.h>
#include <core/rhi/gpuDevice.h>

#include <vector>
#include <vulkan/vulkan.h>

namespace core::rhi
{
	struct Shader::Impl
	{
	public:
		explicit Impl(GpuDevice* device, ShaderDesc& desc);
		~Impl() noexcept;

		void Bind(CommandBuffer* cmd);

		const std::vector<VkPipelineShaderStageCreateInfo>& GetVkStages() const { return m_stages; }
		const std::vector<VkShaderModule>& GetShaderModules() const { return m_modules; }
	private:
		GpuDevice* m_device = nullptr;
		std::vector<VkShaderModule> m_modules;
		std::vector<VkPipelineShaderStageCreateInfo> m_stages;
	};
}

#endif //VRAKTAL_CORE_RHI_SHADER_VK_H
