#ifndef VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
#define VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include <vulkan/vulkan.h> // TODO : Remove this.

#include <core/rhi/buffer.h>
#include <core/rhi/gpuImage.h>

namespace core::rhi
{
    enum class DescriptorType
    {
        UniformBuffer,
        StorageBuffer,
        ImageSampler
    };

    enum class ShaderStage
    {
        Vertex,
        Fragment,
        Compute
    };

    struct DescriptorBinding
    {
        uint32_t binding;
        DescriptorType type;
        ShaderStage stage;
    };

    struct DescriptorSetLayoutDesc
    {
        std::vector<DescriptorBinding> bindings;
    };

    class DescriptorSet
    {
    public:
        struct Impl;

        DescriptorSet(VkDevice device, VkDescriptorPool pool, const DescriptorSetLayoutDesc& layoutDesc); // TODO : Change VkDevice to GpuDevice and VkDescriptorPool to DescriptorPool
        ~DescriptorSet();

        void BindBuffer(uint32_t binding, Buffer* buffer, size_t offset, size_t range);
        void BindImage(uint32_t binding, GpuImage* image, const GpuImageUsage& usage);

        Impl& GetImpl();

    private:
        std::unique_ptr<Impl> m_impl;
    };
}

#endif // VRAKTAL_CORE_RHI_DESCRIPTOR_SET_H
