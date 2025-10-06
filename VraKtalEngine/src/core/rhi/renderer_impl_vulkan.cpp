#include <core/rhi/renderer.h>
#include "../src/core/rhi/renderer_impl_vulkan.h"

#include "../src/core/rhi/commandBuffer_impl_vulkan.h"
#include "../src/core/graphics/mershRenderer_impl_vulkan.h"

#include "../src/core/gpu_details/initializer_vulkan.h"

using namespace core::rhi;
using namespace core::graphics;


Renderer::Renderer(MeshRenderer& _meshRenderer)
{
    m_impl = new Impl(&_meshRenderer);
}

core::rhi::Renderer::~Renderer()
{
    delete m_impl;
}

void Renderer::Render(CommandBuffer& commandBuffer,
    const RenderingInfo& info,
    uint32_t imageIndex,
    const std::vector<core::graphics::GpuMesh>& meshes,
    const glm::mat4& view,
    const glm::mat4& proj) 
{
    m_impl->Render(commandBuffer , info , imageIndex , meshes,view,proj);
}

Renderer::Impl& Renderer::GetImpl()
{
    return *m_impl;
}

Renderer::Impl::Impl(MeshRenderer* meshRenderer) 
    : m_meshRenderer(meshRenderer)
{}

void Renderer::Impl::Render(CommandBuffer& commandBuffer,
    const RenderingInfo& info,
    uint32_t imageIndex,
    const std::vector<core::graphics::GpuMesh>& meshes,
    const glm::mat4& view,
    const glm::mat4& proj)
{
    commandBuffer.BeginRendering(info, imageIndex);

    VkViewport viewport = gpu_details::CreateViewport((float)info.width, (float)info.height);
    VkRect2D scissor = gpu_details::CreateScissor(info.width, info.height);

    vkCmdSetViewport(commandBuffer.GetImpl().GetNative(), 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer.GetImpl().GetNative(), 0, 1, &scissor);

    for (auto& mesh : meshes)
    {
        glm::mat4 model = glm::mat4(1.0f);
        m_meshRenderer->Draw(commandBuffer, mesh, model, view, proj);
    }

    commandBuffer.EndRendering(imageIndex);
}
