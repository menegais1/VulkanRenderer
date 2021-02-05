#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define PI 3.14159265359

#include <GLFW/glfw3.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "Refactoring/VulkanSetup.h"
#include "MemoryManagement/PassThroughAllocator.h"
#include "MemoryManagement/VideoMemoryAllocator.h"
#include "Refactoring/CreateInfoHelpers.h"
#include "FileManagers/FileLoader.h"
#include "Refactoring/RenderFrame.h"
#include "Refactoring/CommandBufferUtils.h"
#include "Refactoring/VulkanCreateFunctions.h"
#include "AutoShaders/AutoShaders.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstring>
#include "FileManagers/Stb/stb_image.h"

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

    VertexInput(const glm::vec3 &position, const glm::vec4 &color) : position(position), color(color) {}
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
    VideoMemoryAllocator vma = VideoMemoryAllocator(vkDevice, physicalDeviceInfo, new PassThroughAllocator());
    VkRenderPass defaultRenderPass;
    VkQueue graphicsQueue, presentationQueue, transferQueue, computeQueue;


    vkGetDeviceQueue(vkDevice, physicalDeviceInfo.queueFamilies.graphicsFamily, 0, &graphicsQueue);
    vkGetDeviceQueue(vkDevice, physicalDeviceInfo.queueFamilies.computeFamily, 1, &computeQueue);
    vkGetDeviceQueue(vkDevice, physicalDeviceInfo.queueFamilies.presentationFamily, 0, &presentationQueue);
    vkGetDeviceQueue(vkDevice, physicalDeviceInfo.queueFamilies.transferFamily, 0, &transferQueue);


    VkAttachmentDescription colorAttachment = vk::attachmentDescription(presentationEngineInfo.format.format, VK_SAMPLE_COUNT_1_BIT,
                                                                        VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE,
                                                                        VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
    VkAttachmentReference colorAttachmentReference = vk::attachmentReference(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    std::vector<VkAttachmentReference> attachmentReferences = {colorAttachmentReference};
    VkSubpassDescription colorSubpass = vk::subpassDescription(VK_PIPELINE_BIND_POINT_GRAPHICS, attachmentReferences, {}, {}, {}, {});
    VkSubpassDependency externalDependency = vk::subpassDependency(VK_SUBPASS_EXTERNAL, 0, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                                                                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, 0,
                                                                   VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, 0);
    std::vector<VkAttachmentDescription> attachments = {colorAttachment};
    std::vector<VkSubpassDependency> dependencies = {externalDependency};
    std::vector<VkSubpassDescription> subpasses = {colorSubpass};
    VkRenderPassCreateInfo defaultRenderPassCreateInfo = vk::renderPassCreateInfo(attachments, dependencies, subpasses);
    vkCreateRenderPass(vkDevice, &defaultRenderPassCreateInfo, nullptr, &defaultRenderPass);


    VkVertexInputBindingDescription vertexInputBindingDescription = vk::vertexInputBindingDescription(0, sizeof(VertexInput), VK_VERTEX_INPUT_RATE_VERTEX);
    VkVertexInputAttributeDescription attributeDescriptionPosition = vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0, offsetof(VertexInput, position));
    VkVertexInputAttributeDescription attributeDescriptionColor = vk::vertexInputAttributeDescription(0, VK_FORMAT_R32G32B32A32_SFLOAT, 1, offsetof(VertexInput, color));
    std::vector<VkVertexInputBindingDescription> bindingsDescriptions = {vertexInputBindingDescription};
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {attributeDescriptionPosition, attributeDescriptionColor};
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

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = vk::pipelineLayoutCreateInfo({}, {});
    VkPipelineLayout pipelineLayout;
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
    VkPipeline defaultPipeline;
    vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &defaultPipeline);

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


    std::vector<VertexInput> vertexInputs = {
            VertexInput(glm::vec3(0, 0, 0), glm::vec4(1, 0, 0, 1)),
            VertexInput(glm::vec3(0.5, 0.5, 0), glm::vec4(0, 1, 0, 1)),
            VertexInput(glm::vec3(0.5, 0, 0), glm::vec4(0, 0, 1, 1))
    };

    std::vector<uint32_t> vertexAndIndicesQueues = {physicalDeviceInfo.queueFamilies.graphicsFamily};
    VkDeviceSize memorySize = sizeof(VertexInput) * vertexInputs.size();
    VkBuffer vertexBuffer = vk::createBuffer(vkDevice, vk::bufferCreateInfo(memorySize, vertexAndIndicesQueues,
                                                                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, 0));
    VkMemoryRequirements vertexBufferMemoryRequirements = vk::getBufferMemoryRequirements(vkDevice, vertexBuffer);
    AllocationBlock vertexBufferMemory = vma.vmalloc(vertexBufferMemoryRequirements, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkBindBufferMemory(vkDevice, vertexBuffer, vertexBufferMemory.vkDeviceMemory, vertexBufferMemory.vkOffset);
    void *memoryMapping;
    vkMapMemory(vkDevice, vertexBufferMemory.vkDeviceMemory, vertexBufferMemory.vkOffset, memorySize, 0, &memoryMapping);
    memcpy(memoryMapping, vertexInputs.data(), memorySize);
//    VkMappedMemoryRange vkMappedMemoryRange = vk::mappedMemoryRange(vertexBufferMemory.vkDeviceMemory,vertexBufferMemory.vkOffset, VK_WHOLE_SIZE);
//    vkFlushMappedMemoryRanges(vkDevice, 1, &vkMappedMemoryRange);

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
            std::vector<VkImageView> framebufferAttachments = {swapchainImageViews[swapchainImageIndex]};
            currentFrame.frameBuffer = vk::createFramebuffer(vkDevice, vk::framebufferCreateInfo(0, defaultRenderPass, framebufferAttachments,
                                                                                                 presentationEngineInfo.extents.width, presentationEngineInfo.extents.height, 1));
            VkRenderPassBeginInfo defaultRenderPassBeginInfo = vk::renderPassBeginInfo(defaultRenderPass, currentFrame.frameBuffer,
                                                                                       vk::rect2D(presentationEngineInfo.extents, {0, 0}), clearValues);
            vk::CommandBufferUtils::beginCommandBuffer(vkDevice, currentFrame.commandBuffer, 0);
            {
                vkCmdBeginRenderPass(currentFrame.commandBuffer, &defaultRenderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
                vkCmdBindPipeline(currentFrame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, defaultPipeline);
                VkDeviceSize offsets = 0;
                vkCmdBindVertexBuffers(currentFrame.commandBuffer,0,1, &vertexBuffer,&offsets);
                vkCmdDraw(currentFrame.commandBuffer, 3, 1, 0, 0);
                vkCmdEndRenderPass(currentFrame.commandBuffer);
            }
            vk::CommandBufferUtils::submitCommandBuffer(graphicsQueue, currentFrame.commandBuffer, {currentFrame.imageReadySemaphore},
                                                        {currentFrame.presentationReadySemaphore}, &waitDstStageFlagBits,
                                                        currentFrame.bufferFinishedFence);

            std::vector<VkSemaphore> waitSemaphores = {currentFrame.presentationReadySemaphore};
            std::vector<VkSwapchainKHR> swapchains = {vkSwapchainKHR};
            VkPresentInfoKHR vkPresentInfoKhr = vk::presentInfoKHR(waitSemaphores, swapchains, &swapchainImageIndex, nullptr);
            vk::VK_ASSERT(vkQueuePresentKHR(presentationQueue, &vkPresentInfoKhr));


        }

    }


    vkDestroySwapchainKHR(vkDevice, vkSwapchainKHR, nullptr);
    vkDestroyDevice(vkDevice, nullptr);
    vkDestroySurfaceKHR(vkInstance, vkSurfaceKHR, nullptr);
    vkDestroyInstance(vkInstance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}
