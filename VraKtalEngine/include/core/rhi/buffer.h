#ifndef VRAKTAL_CORE_RHI_BUFFER_H
#define VRAKTAL_CORE_RHI_BUFFER_H
#pragma once

#include <memory>
#include <cstddef>

namespace core::rhi
{
    class GpuDevice;

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
        size_t size = 0;
        BufferUsage usage = BufferUsage::Vertex;
        MemoryUsage memory = MemoryUsage::GPU;
    };

    class Buffer
    {
    public:
        explicit Buffer(const BufferDesc& desc, GpuDevice& device);
        virtual ~Buffer();

        void* Map();
        void Unmap();
        void Update(const void* data, size_t size, size_t offset = 0);

        const BufferDesc& GetDesc() const { return m_desc; }

        struct Impl;
        Impl& GetImpl();

    private:
        std::unique_ptr<Impl> m_impl;
        BufferDesc m_desc;
    };
}

#endif // VRAKTAL_CORE_RHI_BUFFER_H
