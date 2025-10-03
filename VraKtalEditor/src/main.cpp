#include <core/window.h>
#include <core/rhi/gpuDevice.h>
#include <core/rhi/renderer.h>

#include <utils/imgui_vulkan.h>

//#include "../src/core/rhi/commandBuffer_impl_vulkan.h"
#include "../src/core/graphics/mershRenderer_impl_vulkan.h"


#include <core/graphics/meshRenderer.h>

#include <core/loaders/gltfLoader.h>
#include <core/loaders/textureLoader.h>
#include <core/loaders/shaderLoader.h>

#include <core/rhi/renderingInfo.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>

#pragma comment(lib, "VraKtalEngine_Debug.lib")

using namespace core;
using namespace core::rhi;
using namespace core::loaders;
using namespace core::graphics;
using namespace core::graphics::resources;

int main()
{
    Window window(800, 600, "Textured Mesh");
    GpuDevice device(window);

    //auto& a = device.WrapSwapChain; // TODO : wrap swapchain images, need to expose

    MeshRenderer meshRenderer(device);
    Renderer renderer(meshRenderer);
    // TODO : Add constructor to renderer.h 

    Scene scene = LoadScene("../bin/assets/meshes/traffic_cone/traffic_cone.gltf");

    TextureLoader texLoader(device); // TODO : Change required device from GpuDeviceVulkan to GpuDevice.
    std::vector<TextureGpu> gpuTextures;
    texLoader.LoadSceneTextures(scene, gpuTextures);

    if (scene.materials.empty())    //<--------------------------------------Maybe move this in the scene it self ? 
    {
        resources::Material defMat;
        defMat.baseColorTexture = gpuTextures.empty() ? -1 : 0;
        scene.materials.push_back(defMat);
        std::cout << "Added fallback material" << std::endl;
    }

    for (auto& m : scene.meshes)    //<--------------------------------------Maybe move this in the scene it self ? 
    {
        if ((int)m.materialIndex < 0) 
        {
            m.materialIndex = 0;
        }
    }

    meshRenderer.GetImpl().CreateDescriptorPool((uint32_t)scene.materials.size());

    std::vector<VkDescriptorSet> materialSets;
    materialSets.reserve(scene.materials.size());

    for (auto& mat : scene.materials) //Move into a LoadScene ? / prepare scene ? 
    {
        int texIndex = mat.baseColorTexture;
        if (texIndex < 0 || texIndex >= (int)gpuTextures.size()) 
        {
            texIndex = 0;
        }
        auto& t = gpuTextures[texIndex];
        materialSets.push_back(meshRenderer.GetImpl().CreateDescriptorSet(t.view, t.sampler));
    }

    meshRenderer.GetImpl().SetMaterialDescriptorSets(materialSets);

    std::vector<GpuMesh> gpuMeshes; //< -- - please delete it
    gpuMeshes.reserve(scene.meshes.size());
    for (auto& m : scene.meshes) 
    {
        gpuMeshes.push_back(meshRenderer.UploadMesh(m));
    }

    std::vector<CommandBuffer*> commandBuffers;
    for (int i = 0; i < 2; ++i) 
    {
        commandBuffers.push_back(new CommandBuffer(device));
    }

    uint32_t currentFrame = 0;

    glm::mat4 view = glm::lookAt(glm::vec3(-30, -30, 20), glm::vec3(0, 10, 0), glm::vec3(0, 1, 0));

    auto size = window.Size();
    float aspect = static_cast<float>(size.first) / static_cast<float>(size.second);

    glm::mat4 proj = glm::perspectiveRH_ZO(glm::radians(60.0f), aspect, 0.1f, 100.0f);
    proj[1][1] *= -1;

    utils::InitImGui(device, window.GlfwHandle());

    while (!window.ShouldClose())
    {
        window.PollEvents();

        uint32_t imageIndex;
        if (!device.BeginFrame(imageIndex)) 
        {
            continue;
        }
        //
        CommandBuffer& commandBuffer = *commandBuffers[currentFrame];
        commandBuffer.Begin();

        RenderingInfo info;
        info.width = device.GetSize().first;
        info.height = device.GetSize().second;

        RenderingAttachment color{};
        color.image = device.GetSwapchainImage(imageIndex);
        color.clearValue = { 0.1f, 0.1f, 0.15f, 1.f };
        color.loadOp = LoadOp::Clear;
        color.storeOp = StoreOp::Store;
        info.colorAttachments.push_back(color);

        // TODO : Expose swapchainImages

        renderer.Render(commandBuffer, info, imageIndex, gpuMeshes, view, proj);

        commandBuffer.End();
        device.EndFrame(imageIndex, commandBuffer);
        // TODO : Expose endFrame

        currentFrame = (currentFrame + 1) % 2;
    }

    device.WaitIdle();

    for (auto& g : gpuMeshes)
    {
        meshRenderer.DestroyMesh(g);
    }
    for (auto* cmd : commandBuffers)
    {
        delete cmd;
    }

    std::cout << "Done\n";
    return 0;
}