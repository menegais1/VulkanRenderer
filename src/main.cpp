#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define STB_IMAGE_IMPLEMENTATION
#define PI 3.14159265359

#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "Refactoring/VulkanSetup.h"
#include "MemoryManagement/PassThroughAllocator.h"
#include "Refactoring/CreateInfoHelpers.h"
#include "FileManagers/FileLoader.h"
#include "Refactoring/RenderFrame.h"
#include "Refactoring/CommandBufferUtils.h"
#include "Refactoring/VulkanCreateFunctions.h"
#include "AutoShaders/AutoShaders.h"
#include "VulkanGui.h"
#include "MemoryManagement/VMA.h"
#include "Refactoring/Buffer.h"
#include "Refactoring/Queue.h"
#include "Refactoring/HostDeviceTransfer.h"
#include "Refactoring/Texture2D/Image2D.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstring>
#include <glm/vec2.hpp>
#include "../Dependencies/stb/stb_image.h"
#include "Refactoring/CommandBuffer.h"

int const WIDTH = 500;
int const HEIGHT = 500;

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {

}

void mouseButton(GLFWwindow *window, int button, int action, int modifier) {
}

void mouseMovement(GLFWwindow *window, double xpos, double ypos) {

}

struct VertexInput {
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 uv;

    VertexInput(const glm::vec3 &position, const glm::vec4 &color, const glm::vec2 &uv) : position(position), color(color), uv(uv) {}
};

std::vector<char> loadShader(const std::string filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "CANNOT OPEN SHADER FILE" << std::endl;
        exit(-1);
    }
    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();
    return buffer;
}

GLFWwindow *setupGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Render", nullptr, nullptr);
    glfwSetKeyCallback(window, keyboard);
    glfwSetMouseButtonCallback(window, mouseButton);
    glfwSetCursorPosCallback(window, mouseMovement);
    return window;
}

VkRenderPass createDefaultRenderPass(VkDevice vkDevice, PresentationEngineInfo presentationEngineInfo) {
    VkAttachmentDescription colorAttachment = vk::attachmentDescription(presentationEngineInfo.format.format,
                                                                        VK_SAMPLE_COUNT_1_BIT,
                                                                        VK_ATTACHMENT_LOAD_OP_CLEAR,
                                                                        VK_ATTACHMENT_STORE_OP_STORE,
                                                                        VK_IMAGE_LAYOUT_UNDEFINED,
                                                                        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    VkAttachmentReference colorAttachmentReference = vk::attachmentReference(0,
                                                                             VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    std::vector<VkAttachmentReference> attachmentReferences = {colorAttachmentReference};
    VkSubpassDescription colorSubpass = vk::subpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS,
                                                               attachmentReferences, {}, {}, {}, {});
    VkSubpassDependency externalDependency = vk::subpassDependency(VK_SUBPASS_EXTERNAL, 0,
                                                                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
                                                                   VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0);
    std::vector<VkAttachmentDescription> attachments = {colorAttachment};
    std::vector<VkSubpassDependency> dependencies = {externalDependency};
    std::vector<VkSubpassDescription> subpasses = {colorSubpass};
    VkRenderPassCreateInfo defaultRenderPassCreateInfo = vk::renderPassCreateInfo(attachments, dependencies,
                                                                                  subpasses);
    return vk::createRenderPass(vkDevice, vk::renderPassCreateInfo(attachments, dependencies, subpasses));
}


int main() {

    AutoShaders::compile();
    GLFWwindow *window = setupGLFW();
    VulkanSetup vulkanSetup(window);
    VkDevice vkDevice = vulkanSetup.vkDevice;
    VkInstance vkInstance = vulkanSetup.vkInstance;
    VkSwapchainKHR vkSwapchainKHR = vulkanSetup.vkSwapchainKHR;
    VkSurfaceKHR vkSurfaceKHR = vulkanSetup.vkSurfaceKHR;
    PhysicalDeviceInfo physicalDeviceInfo = vulkanSetup.physicalDeviceInfo;
    PresentationEngineInfo presentationEngineInfo = vulkanSetup.presentationEngineInfo;
    VkPipeline defaultPipeline;

    VMA::getInstance().initialize(vkDevice, physicalDeviceInfo, new PassThroughAllocator());
    vk::Queue graphicsQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.graphicsFamily, 0);
    vk::Queue computeQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.computeFamily, 1);
    vk::Queue presentationQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.presentationFamily, 0);
    vk::Queue transferQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.transferFamily, 0);


    // CREATE DESCRIPTOR SETS LAYOUT
    VkDescriptorSetLayoutBinding textureDescriptorSetBinding = vk::descriptorSetLayoutBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT);
    std::vector<VkDescriptorSetLayoutBinding> textureDescriptorBindings = {textureDescriptorSetBinding};
    VkDescriptorSetLayout textureDescriptorSetLayout = vk::createDescriptorSetLayout(vkDevice, vk::descriptorSetLayoutCreateInfo(textureDescriptorBindings));
    std::vector<VkDescriptorSetLayout> textureDescriptorSetLayouts = {textureDescriptorSetLayout};
    // END

    VkPipelineLayout pipelineLayout;

    VkRenderPass defaultRenderPass = createDefaultRenderPass(vkDevice, presentationEngineInfo);
    VkRenderPass imGuiRenderPass = VulkanGui::ImGuiCreateRenderPass(vkDevice, presentationEngineInfo);
    // Pipeline Creation
    {
        VkVertexInputBindingDescription vertexInputBindingDescription = vk::vertexInputBindingDescription(0, sizeof(VertexInput), VK_VERTEX_INPUT_RATE_VERTEX);
        VkVertexInputAttributeDescription attributeDescriptionPosition = vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0, offsetof(VertexInput, position));
        VkVertexInputAttributeDescription attributeDescriptionColor = vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32A32_SFLOAT, 1, offsetof(VertexInput, color));
        VkVertexInputAttributeDescription attributeDescriptionUv = vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32_SFLOAT, 2, offsetof(VertexInput, uv));
        std::vector<VkVertexInputBindingDescription> bindingsDescriptions = {vertexInputBindingDescription};
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {attributeDescriptionPosition, attributeDescriptionColor, attributeDescriptionUv};
        VkPipelineVertexInputStateCreateInfo vertexInputState = vk::pipelineVertexInputStateCreateInfo(bindingsDescriptions, attributeDescriptions);

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = vk::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
        VkPipelineRasterizationStateCreateInfo rasterizationState = vk::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT,
                                                                                                             VK_FRONT_FACE_COUNTER_CLOCKWISE, 1.0);
        VkPipelineDepthStencilStateCreateInfo depthStencilState = vk::pipelineDepthStencilStateCreateInfo(VK_FALSE, VK_FALSE, VK_COMPARE_OP_NEVER);
        VkPipelineTessellationStateCreateInfo tesselationState = vk::pipelineTessellationStateCreateInfo(0);

        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = vk::pipelineColorBlendAttachmentState();
        colorBlendAttachmentState.blendEnable = VK_FALSE;
        colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_R_BIT |
                                                   VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;


        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = vk::pipelineLayoutCreateInfo(textureDescriptorSetLayouts, {});
        vkCreatePipelineLayout(vkDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
        float blendConstants[4] = {1, 1, 1, 1};
        std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments = {colorBlendAttachmentState};
        VkPipelineColorBlendStateCreateInfo colorBlendState = vk::pipelineColorBlendStateCreateInfo(colorBlendAttachments, blendConstants, VK_FALSE, VK_LOGIC_OP_NO_OP);
        auto vertexBytes = loadShader(FileLoader::getPath("Shaders/Compiled/vulkanBase.vert.spv"));
        auto fragmentBytes = loadShader(FileLoader::getPath("Shaders/Compiled/vulkanBase.frag.spv"));
        VkShaderModuleCreateInfo vertexShaderCreateInfo = vk::shaderModuleCreateInfo(vertexBytes);
        VkShaderModuleCreateInfo fragmentShaderCreateInfo = vk::shaderModuleCreateInfo(fragmentBytes);

        VkShaderModule vertexShader, fragmentShader;
        vkCreateShaderModule(vkDevice, &vertexShaderCreateInfo, nullptr, &vertexShader);
        vkCreateShaderModule(vkDevice, &fragmentShaderCreateInfo, nullptr, &fragmentShader);

        VkPipelineShaderStageCreateInfo vertexShaderStage = vk::pipelineShaderStageCreateInfo(vertexShader, "main", VK_SHADER_STAGE_VERTEX_BIT);
        VkPipelineShaderStageCreateInfo fragmentShaderStage = vk::pipelineShaderStageCreateInfo(fragmentShader, "main", VK_SHADER_STAGE_FRAGMENT_BIT);

        std::vector<VkViewport> viewports = {vk::viewport(0, 0, presentationEngineInfo.extents.width, presentationEngineInfo.extents.height, 0, 1)};
        std::vector<VkRect2D> scissors = {vk::rect2D(vk::extent2D(presentationEngineInfo.extents.width, presentationEngineInfo.extents.height), vk::offset2D(0, 0))};
        VkPipelineViewportStateCreateInfo viewportState = vk::pipelineViewportStateCreateInfo(viewports, scissors);

        VkPipelineMultisampleStateCreateInfo multisampleState = vk::pipelineMultisampleStateCreateInfo();
        multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampleState.sampleShadingEnable = VK_FALSE;
        std::vector<VkPipelineShaderStageCreateInfo> stages = {vertexShaderStage, fragmentShaderStage};
        VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = vk::graphicsPipelineCreateInfo(stages, &vertexInputState, &inputAssemblyState, &tesselationState, &viewportState,
                                                                                                 &rasterizationState, &multisampleState, &depthStencilState,
                                                                                                 &colorBlendState, nullptr, pipelineLayout, defaultRenderPass, 0);
        defaultPipeline = vk::createGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo)[0];
    }
    std::vector<VkImage> swapchainImages;
    uint32_t swapchainImagesCount = 0;
    vkGetSwapchainImagesKHR(vkDevice, vkSwapchainKHR, &swapchainImagesCount, nullptr);
    swapchainImages.resize(swapchainImagesCount);
    vkGetSwapchainImagesKHR(vkDevice, vkSwapchainKHR, &swapchainImagesCount, swapchainImages.data());
    std::cout << "Swapchain images: " << swapchainImagesCount << std::endl;
    std::cout << "Presentation Engine images: " << presentationEngineInfo.imageCount << std::endl;
    presentationEngineInfo.imageCount = swapchainImagesCount;
    std::vector<VkImageView> swapchainImageViews(swapchainImagesCount);
    VkImageSubresourceRange swapchainImageViewSubresourceRange = vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
    for (int i = 0; i < swapchainImagesCount; i++) {
        VkImageViewCreateInfo vkImageViewCreateInfo = vk::imageViewCreateInfo(swapchainImages[i], VK_IMAGE_VIEW_TYPE_2D, presentationEngineInfo.format.format,
                                                                              {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                               VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY}, swapchainImageViewSubresourceRange);
        vkCreateImageView(vkDevice, &vkImageViewCreateInfo, nullptr, &swapchainImageViews[i]);
    }

    VkCommandPool graphicsPool = vk::createCommandPool(vkDevice, vk::commandPoolCreateInfo(physicalDeviceInfo.queueFamilies.graphicsFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT));


    uint32_t renderFramesAmount = 2;
    VkCommandBuffer *graphicsCommandBuffers = {vk::allocateCommandBuffers(vkDevice,
                                                                          vk::commandBufferAllocateInfo(graphicsPool, renderFramesAmount, VK_COMMAND_BUFFER_LEVEL_PRIMARY),
                                                                          renderFramesAmount)};

    std::vector<RenderFrame> renderFrames(renderFramesAmount);
    for (int i = 0; i < renderFramesAmount; ++i) {
        RenderFrame renderFrame{};
        renderFrame.commandBuffer = graphicsCommandBuffers[i];
        renderFrame.imageReadySemaphore = vk::createSemaphore(vkDevice, vk::semaphoreCreateInfo());
        renderFrame.presentationReadySemaphore = vk::createSemaphore(vkDevice, vk::semaphoreCreateInfo());
        renderFrame.bufferFinishedFence = vk::createFence(vkDevice, vk::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT));
        renderFrame.frameBuffer = VK_NULL_HANDLE;
        renderFrames[i] = renderFrame;
    }
    VulkanGui::ImGuiSetupForVulkan(window, vulkanSetup, 0, graphicsQueue.queue, imGuiRenderPass, graphicsPool);

    vk::HostDeviceTransfer hostDeviceTransfer = vk::HostDeviceTransfer(vkDevice, transferQueue);

    std::vector<VertexInput> vertexInputs = {
            VertexInput(glm::vec3(0, 0, 0), glm::vec4(1, 0, 0, 1), glm::vec2(0, 0)),
            VertexInput(glm::vec3(0.5, 0.5, 0), glm::vec4(0, 1, 0, 1), glm::vec2(1, 1)),
            VertexInput(glm::vec3(0.5, 0, 0), glm::vec4(0, 0, 1, 1), glm::vec2(1, 0)),
            VertexInput(glm::vec3(0, 0.5, 0), glm::vec4(0, 0, 1, 1), glm::vec2(0, 1))
    };

    std::vector<uint32_t> indexVector = {
            0, 1, 2, 0, 3, 1
    };

    std::vector<uint32_t> graphicsAndTransferQueues = {physicalDeviceInfo.queueFamilies.graphicsFamily, physicalDeviceInfo.queueFamilies.transferFamily};

    vk::Buffer vertexBuffer = vk::Buffer(vkDevice, sizeof(VertexInput) * vertexInputs.size(), graphicsAndTransferQueues,
                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_CONCURRENT, 0,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    hostDeviceTransfer.transferBuffer(vertexBuffer.size, vertexInputs.data(), vertexBuffer);
    vk::Buffer indexBuffer = vk::Buffer(vkDevice, indexVector.size() * sizeof(uint32_t), graphicsAndTransferQueues,
                                        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                        VK_SHARING_MODE_CONCURRENT, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    hostDeviceTransfer.transferBuffer(indexBuffer.size, indexVector.data(), indexBuffer);

    vk::CommandBuffer commandBuffer = vk::CommandBuffer(vkDevice, graphicsQueue, graphicsPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    VkImage testImage;
    VkImageView testImageView;
    VkSampler testSampler;
    {
        int width, height, channels = 0;
        unsigned char *result = stbi_load(FileLoader::getPath("Resources/cat.bmp").c_str(), &width, &height, &channels, 0);
        float *convertedResult = new float[width * height * channels];
        for (int i = 0; i < width * height; i++) {
            for (int j = 0; j < channels; j++) {
                convertedResult[(i * 4) + j] = result[i];
            }
        }
        std::cout << "Loaded image" << std::endl;
        std::cout << "w:" << width << " h:" << height << " c:" << channels << std::endl;
        testImage = vk::createImage(vkDevice, vk::imageCreateInfo(0, VK_IMAGE_TYPE_2D, VK_FORMAT_R32G32B32A32_SFLOAT,
                                                                  vk::extent3D(width, height, 1), 1, 1, VK_SAMPLE_COUNT_1_BIT,
                                                                  VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                                                  VK_SHARING_MODE_CONCURRENT, graphicsAndTransferQueues, VK_IMAGE_LAYOUT_UNDEFINED));
        VkMemoryRequirements requirements = vk::getImageMemoryRequirements(vkDevice, testImage);
        AllocationBlock memory = VMA::getInstance().vmalloc(requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkBindImageMemory(vkDevice, testImage, memory.vkDeviceMemory, memory.vkOffset);
        testImageView = vk::createImageView(vkDevice,
                                            vk::imageViewCreateInfo(testImage, VK_IMAGE_VIEW_TYPE_2D,
                                                                    VK_FORMAT_R32G32B32A32_SFLOAT,
                                                                    {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                                                                     VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                    vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT,
                                                                                              0, 1, 0, 1)));
        VkSamplerCreateInfo samplerCreateInfo = vk::samplerCreateInfo();
        samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
        samplerCreateInfo.compareEnable = VK_FALSE;
        samplerCreateInfo.anisotropyEnable = VK_FALSE;
        samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        testSampler = vk::createSampler(vkDevice, samplerCreateInfo);

        hostDeviceTransfer.submitOneTimeTransferBuffer([&testImage](VkCommandBuffer transferBuffer) {
            VkImageMemoryBarrier transferSrcMemoryBarrier = vk::imageMemoryBarrier(0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
                                                                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                                                                   testImage, vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0,
                                                                                                                        1, 0, 1));
            vkCmdPipelineBarrier(transferBuffer, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                                 0, nullptr,
                                 0, nullptr, 1, &transferSrcMemoryBarrier);
        });

        VkPipelineStageFlags waitDstFlags = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        commandBuffer.submitOneTime({}, {}, &waitDstFlags, [&testImage](VkCommandBuffer commandBuffer) {
            VkImageMemoryBarrier imageReadMemoryBarrier = vk::imageMemoryBarrier(VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                                                                 VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_QUEUE_FAMILY_IGNORED, VK_QUEUE_FAMILY_IGNORED,
                                                                                 testImage, vk::imageSubresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0,
                                                                                                                      1, 0, 1));
            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                                 0, nullptr,
                                 0, nullptr, 1, &imageReadMemoryBarrier);
        });
        hostDeviceTransfer.transferImageFromBuffer(width, height, channels, 4, convertedResult, testImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    }

    // CREATE DESCRIPTORS POOLS
    VkDescriptorPoolSize textureDescriptorPoolSize = vk::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100);
    std::vector<VkDescriptorPoolSize> descriptorPoolSizes = {textureDescriptorPoolSize};
    VkDescriptorPool textureDescriptorPool = vk::createDescriptorPool(vkDevice, vk::descriptorPoolCreateInfo(descriptorPoolSizes, 100));
    // END
    //CREATE AND UPDATE DESCRIPTOR SETS
    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo = vk::descriptorSetAllocateInfo(textureDescriptorPool, textureDescriptorSetLayouts);
    VkDescriptorSet textureDescriptorSet = vk::allocateDescriptorSets(vkDevice, 1, &vkDescriptorSetAllocateInfo)[0];
    std::vector<VkDescriptorSet> descriptorSets = {textureDescriptorSet};
    VkDescriptorImageInfo vkTextureImageInfo = vk::descriptorImageInfo(testSampler, testImageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    std::vector<VkDescriptorImageInfo> descriptorImageInfos = {vkTextureImageInfo};
    VkWriteDescriptorSet textureDescriptorSetWrite = vk::writeDescriptorSet(descriptorImageInfos, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, textureDescriptorSet, 0, 0);
    vkUpdateDescriptorSets(vkDevice, 1, &textureDescriptorSetWrite, 0, nullptr);
    //END

    VkClearValue colorBufferClearValue;
    colorBufferClearValue.color = {1, 0, 0, 1};
    std::vector<VkClearValue> clearValues = {colorBufferClearValue};
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        for (int i = 0; i < renderFramesAmount; i++) {
            uint32_t swapchainImageIndex = -1;
            VkPipelineStageFlags waitDstStageFlagBits = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            RenderFrame currentFrame = renderFrames[i];
            vk::CommandBufferUtils::waitForFences(vkDevice, {currentFrame.bufferFinishedFence}, true);
            vkAcquireNextImageKHR(vkDevice, vkSwapchainKHR, UINT64_MAX, currentFrame.imageReadySemaphore, VK_NULL_HANDLE, &swapchainImageIndex);
            if (currentFrame.frameBuffer != VK_NULL_HANDLE) {
                vkDestroyFramebuffer(vkDevice, currentFrame.frameBuffer, nullptr);
            }
            ImGui_ImplVulkan_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow();
            ImGui::Render();

            std::vector<VkImageView> framebufferAttachments = {swapchainImageViews[swapchainImageIndex]};
            currentFrame.frameBuffer = vk::createFramebuffer(vkDevice, vk::framebufferCreateInfo(0, defaultRenderPass, framebufferAttachments,
                                                                                                 presentationEngineInfo.extents.width, presentationEngineInfo.extents.height, 1));
            VkRenderPassBeginInfo defaultRenderPassBeginInfo = vk::renderPassBeginInfo(defaultRenderPass, currentFrame.frameBuffer,
                                                                                       vk::rect2D(presentationEngineInfo.extents, {0, 0}), clearValues);
            VkRenderPassBeginInfo imGuiRenderPassBeginInfo = vk::renderPassBeginInfo(imGuiRenderPass, currentFrame.frameBuffer,
                                                                                     vk::rect2D(presentationEngineInfo.extents, {0, 0}), clearValues);
            vk::CommandBufferUtils::beginCommandBuffer(vkDevice, currentFrame.commandBuffer, 0);
            {
                vkCmdBeginRenderPass(currentFrame.commandBuffer, &defaultRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
                {
                    vkCmdBindPipeline(currentFrame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, defaultPipeline);
                    VkDeviceSize offsets = 0;
                    vkCmdBindVertexBuffers(currentFrame.commandBuffer, 0, 1, &vertexBuffer.buffer, &offsets);
                    vkCmdBindIndexBuffer(currentFrame.commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);
                    vkCmdBindDescriptorSets(currentFrame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &textureDescriptorSet, 0, nullptr);
                    vkCmdDrawIndexed(currentFrame.commandBuffer, indexVector.size(), 1, 0, 0, 0);
                }
                vkCmdEndRenderPass(currentFrame.commandBuffer);
                vkCmdBeginRenderPass(currentFrame.commandBuffer, &imGuiRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
                {
                    vkCmdBindPipeline(currentFrame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, defaultPipeline);
                    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), currentFrame.commandBuffer);
                }
                vkCmdEndRenderPass(currentFrame.commandBuffer);
            }
            vk::CommandBufferUtils::submitCommandBuffer(graphicsQueue.queue, currentFrame.commandBuffer, {currentFrame.imageReadySemaphore},
                                                        {currentFrame.presentationReadySemaphore}, &waitDstStageFlagBits,
                                                        currentFrame.bufferFinishedFence);

            std::vector<VkSemaphore> waitSemaphores = {currentFrame.presentationReadySemaphore};
            std::vector<VkSwapchainKHR> swapchains = {vkSwapchainKHR};
            VkPresentInfoKHR vkPresentInfoKhr = vk::presentInfoKHR(waitSemaphores, swapchains, &swapchainImageIndex, nullptr);
            vk::VK_ASSERT(vkQueuePresentKHR(presentationQueue.queue, &vkPresentInfoKhr));


        }

    }


    vkDestroySwapchainKHR(vkDevice, vkSwapchainKHR, nullptr);
    vkDestroyDevice(vkDevice, nullptr);
    vkDestroySurfaceKHR(vkInstance, vkSurfaceKHR, nullptr);
    vkDestroyInstance(vkInstance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}