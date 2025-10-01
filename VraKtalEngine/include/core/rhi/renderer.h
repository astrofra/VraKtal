#ifndef VRAKTAL_CORE_RHI_RENDERER_H
#define VRAKTAL_CORE_RHI_RENDERER_H
#pragma once
#include <core/graphics/meshRenderer.h> // Add this include at the top of the file, after other includes
#include <core/rhi/commandBuffer.h>
#include <core/rhi/renderingInfo.h>

#include <unordered_map>

namespace core::graphics { struct GpuMesh; }

namespace core::rhi
{
    class Renderer
    {
    private:
        struct Impl;
        Impl* m_impl;
    public:
        Renderer(core::graphics::MeshRenderer& _meshRenderer);
        ~Renderer();

        void Render(CommandBuffer& commandBuffer,
            const RenderingInfo& info,
            uint32_t imageIndex,
            const std::vector<core::graphics::GpuMesh>& meshes,
            const glm::mat4& view,
            const glm::mat4& proj);

        Impl& GetImpl();
    };
}

#endif //VRAKTAL_CORE_RHI_RENDERER_H