#ifndef VRAKTAL_CORE_RHI_SHADER_H
#define VRAKTAL_CORE_RHI_SHADER_H
#pragma once

#include <vector>
#include <core/rhi/enums.h>

namespace core::rhi
{
	class CommandBuffer;

	struct ShaderStageDesc
	{
		ShaderStage stage;
		std::vector<uint8_t> code;
	};

	struct ShaderDesc
	{
		std::vector<ShaderStageDesc> stages;
	};

	class Shader
	{
	public:
		Shader();
		void Bind(class CommandBuffer* cmd);
	};
}

#endif //VRAKTAL_CORE_RHI_SHADER_H
