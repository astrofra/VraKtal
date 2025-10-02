#ifndef VRAKTAL_CORE_GRAPHICS_MESH_RENDERER_H
#define VRAKTAL_CORE_GRAPHICS_MESH_RENDERER_H
#pragma once


#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace core::graphics::resources { struct Vertex; struct Material; class Mesh; }

using namespace core;
using namespace core::graphics::resources;

namespace core
{
    namespace rhi
    {
        class CommandBuffer;
        class GpuDevice;
    }

    namespace graphics 
    {
        struct GpuMesh;

        class MeshRenderer
        {
            struct Impl;
            std::unique_ptr<Impl> m_impl;
        public:
            MeshRenderer(rhi::GpuDevice& _device);
            ~MeshRenderer();

            GpuMesh UploadMesh(const Mesh& mesh);
            void DestroyMesh(GpuMesh& mesh);

            void Draw(rhi::CommandBuffer& cmd, const GpuMesh& mesh,
                     const glm::mat4& model = glm::mat4(1.0f),
                     const glm::mat4& view = glm::mat4(1.0f),
                     const glm::mat4& projection = glm::mat4(1.0f));

            rhi::GpuDevice* Device();

			void* GetDescriptorPool(); // TODO : remove

            Impl& GetImpl();
        };
    }
}

#endif //VRAKTAL_CORE_GRAPHICS_MESH_RENDERER_H
