#ifndef VRAKTAL_CORE_RHI_SHADER_H
#define VRAKTAL_CORE_RHI_SHADER_H
#pragma once

#include <vector>
#include <memory>

#include <core/rhi/enums.h>

namespace core::rhi
{
	class CommandBuffer;
	class GpuDevice;

	struct ShaderStageDesc
	{
		ShaderStage stage;
		std::vector<uint8_t> spirv; 
		const char* entryPoint = "main";
	};

	struct ShaderDesc
	{
		std::vector<ShaderStageDesc> stages;
	};

	class Shader
	{
	public:
		Shader(GpuDevice* device, ShaderDesc& desc);
		~Shader();
		void Bind(class CommandBuffer* cmd);

		struct Impl;
		Impl& GetImpl();
	private:
		std::unique_ptr<Impl> m_impl;
	};
}

#endif //VRAKTAL_CORE_RHI_SHADER_H
