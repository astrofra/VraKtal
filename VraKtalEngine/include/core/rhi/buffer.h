#ifndef VRAKTAL_CORE_RHI_BUFFER_H
#define VRAKTAL_CORE_RHI_BUFFER_H
#pragma once

namespace core::rhi
{
	enum class BufferUsage
	{
		Vertex, 
		Index,
		Uniform,
		Storage,
		TransferSrc,
		TransferDst
	};

	enum class MemoryUsage
	{
		CPU,
		GPU
	};

	struct BufferDesc
	{
		size_t size;
		BufferUsage usage;
		MemoryUsage memory;
	};

	class Buffer
	{
	public:
		virtual ~Buffer() = default;

		virtual void* Map() = 0;
		virtual void Unmap() = 0;
		virtual void Update(const void* data, size_t size, size_t offset = 0) = 0;
	};
}

#endif //VRAKTAL_CORE_RHI_BUFFER_H
