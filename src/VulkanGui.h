//
// Created by Lorenzo on 2/5/12021.
//

#ifndef VULKAN_BASE_IMGUI_H
#define VULKAN_BASE_IMGUI_H

#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"
#include "Refactoring/VulkanSetup.h"

class VulkanGui
{
public:

    static VkRenderPass ImGuiCreateRenderPass(VkDevice vkDevice, PresentationEngineInfo presentationEngineInfo){
        VkAttachmentDescription colorAttachment = vk::attachmentDescription(presentationEngineInfo.format.format,
                                                                            VK_SAMPLE_COUNT_1_BIT,
                                                                            VK_ATTACHMENT_LOAD_OP_LOAD,
                                                                            VK_ATTACHMENT_STORE_OP_STORE,
                                                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        VkAttachmentDescription depthAttachment = vk::attachmentDescription(VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                                            VK_SAMPLE_COUNT_1_BIT,
                                                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        VkAttachmentReference colorAttachmentReference = vk::attachmentReference(0,
                                                                                 VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
        VkAttachmentReference depthAttachmentReference = vk::attachmentReference(1,
                                                                                 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
        std::vector<VkAttachmentReference> colorReferences = {colorAttachmentReference};
        std::vector<VkAttachmentReference> depthReferences = {depthAttachmentReference};
        VkSubpassDescription colorSubpass = vk::subpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                                   colorReferences, {}, {}, depthReferences, {});
        VkSubpassDependency externalDependency = vk::subpassDependency(VK_SUBPASS_EXTERNAL, 0,
                                                                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                                       VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
                                                                       VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0);
        std::vector<VkAttachmentDescription> attachments = {colorAttachment,depthAttachment};
        std::vector<VkSubpassDependency> dependencies = {externalDependency};
        std::vector<VkSubpassDescription> subpasses = {colorSubpass};

        return vk::createRenderPass(vkDevice, vk::renderPassCreateInfo(attachments, dependencies, subpasses));
    }
    static void ImGuiSetupForVulkan(GLFWwindow* window, const VulkanSetup& setup, uint32_t queueIndex,
                                    const VkQueue& queue, const VkRenderPass& renderPass,
                                    const VkCommandPool & commandPool)
    {
        /* Setup context */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags = ImGuiConfigFlags_NavEnableKeyboard;

        /* Style */
        ImGui::StyleColorsDark();

        /* Vulkan Setup */
        VkDescriptorPool descriptorPool {};
        CreateDescriptorPools(setup.vkDevice, &descriptorPool);
        ImGui_ImplGlfw_InitForVulkan(window, true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = setup.vkInstance;
        initInfo.PhysicalDevice = setup.vkPhysicalDevice;
        initInfo.Device = setup.vkDevice;
        initInfo.QueueFamily = queueIndex;
        initInfo.Queue = queue;
        initInfo.DescriptorPool = descriptorPool;
        initInfo.PipelineCache = VK_NULL_HANDLE;
        initInfo.Allocator = VK_NULL_HANDLE; // Todo: Add our custom allocator
        initInfo.MinImageCount = 2;
        initInfo.ImageCount = 2;
        initInfo.CheckVkResultFn = nullptr;
        ImGui_ImplVulkan_Init(&initInfo, renderPass);

        VkFenceCreateInfo fenceCreateInfo = vk::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
        VkFence fence = vk::createFence(setup.vkDevice, fenceCreateInfo);
        vkResetFences(setup.vkDevice, 1, &fence);
        VkCommandBufferAllocateInfo v = vk::commandBufferAllocateInfo(commandPool, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        VkCommandBuffer commandBuffer {};
        vkAllocateCommandBuffers(setup.vkDevice, &v, &commandBuffer);
        vk::CommandBufferUtils::beginCommandBuffer(setup.vkDevice, commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        {
            ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        }
        vk::CommandBufferUtils::submitCommandBuffer(queue, commandBuffer, {}, {}, VK_NULL_HANDLE, fence);
        vk::CommandBufferUtils::waitForFences(setup.vkDevice, {fence}, true);
        vkFreeCommandBuffers(setup.vkDevice, commandPool, 1, &commandBuffer);
    }
private:
    static void CreateDescriptorPools(const VkDevice& device, VkDescriptorPool* descriptorPool)
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        vkCreateDescriptorPool(device, &pool_info, VK_NULL_HANDLE, descriptorPool);
    }
};


#endif //VULKAN_BASE_IMGUI_H
