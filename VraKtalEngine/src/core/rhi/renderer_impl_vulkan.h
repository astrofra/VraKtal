#ifndef VRAKTAL_CORE_RHI_RENDERER_VK_H
#define VRAKTAL_CORE_RHI_RENDERER_VK_H
#pragma once

#include <core/graphics/meshRenderer.h>
#include <core/rhi/renderer.h>

using namespace core::graphics;

namespace core::rhi
{

    struct Renderer::Impl
    {
    public:
        explicit Impl(MeshRenderer* meshRenderer);

        void Render(CommandBuffer& commandBuffer,
            const RenderingInfo& info,
            uint32_t imageIndex,
            const std::vector<core::graphics::GpuMesh>& meshes,
            const glm::mat4& view,
            const glm::mat4& proj);

    private:
        MeshRenderer* m_meshRenderer;
    };
};

#endif //VRAKTAL_CORE_RHI_RENDERER_VK_H
