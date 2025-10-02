#pragma once
#include<core/graphics/meshRenderer.h>
#include <core/rhi/gpuDevice.h>

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace core::graphics {

    struct GpuMesh
    {
        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VmaAllocation vertexAlloc = nullptr;
        VkBuffer indexBuffer = VK_NULL_HANDLE;
        VmaAllocation indexAlloc = nullptr;
        uint32_t indexCount = 0;
        int materialIndex = -1;
    };

    struct MeshRenderer::Impl
    {
        Impl(rhi::GpuDevice& device);
        
        void Draw(rhi::CommandBuffer& cmd, const GpuMesh& mesh,
            const glm::mat4& model = glm::mat4(1.0f),
            const glm::mat4& view = glm::mat4(1.0f),
            const glm::mat4& projection = glm::mat4(1.0f));

        GpuMesh UploadMesh(const Mesh& mesh);
        
        VkDescriptorSetLayout DescriptorSetLayout() const { return m_descriptorSetLayout; }
        const VkDevice Device();
        VkDescriptorSet CreateDescriptorSet(VkImageView view, VkSampler sampler);
        
        void CreateDescriptorPool(uint32_t maxSets);
        void SetMaterialDescriptorSets(const std::vector<VkDescriptorSet>& sets);
        void CreateDescriptorSetLayout();
        
        void CreatePipeline();
        void DestroyMesh(GpuMesh& mesh);

		VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }

    private:
        
        void DestroyDescriptors();

        rhi::GpuDevice&     m_device;
        VkPipeline          m_pipeline = VK_NULL_HANDLE;
        VkPipelineLayout    m_pipelineLayout = VK_NULL_HANDLE;

        VkDescriptorSetLayout m_descriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool      m_descriptorPool = VK_NULL_HANDLE;

        std::vector<VkDescriptorSet> m_materialDescriptorSets;
    };
}