#define GLFW_INCLUDE_VULKAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define TINYOBJLOADER_IMPLEMENTATION
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
#include "Refactoring/Queue.h"
#include "Refactoring/HostDeviceTransfer.h"
#include "tiny_obj_loader.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <cstring>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../Dependencies/stb/stb_image.h"
#include "Refactoring/CommandBuffer.h"
#include "Models/ObjectLoader.h"
#include "Resources/ResourcesManager.h"
#include "Textures/Texture.h"
#include "Models/Model.h"
#include "Models/DamagedHelmet.h"
#include "Models/Human.h"
#include "Refactoring/DescriptorSetLayout.h"

int const WIDTH = 1200;
int const HEIGHT = 720;

struct Uniform {
    alignas(4) bool normalMapping;
    alignas(16) glm::vec3 lightPosition = glm::vec3(1.0, 1.0, 1.0);
    alignas(16) glm::vec3 viewPosition;
    alignas(16) glm::vec4 lightColor = glm::vec4(1.0, 1.0, 1.0, 1.0);
    alignas(64) glm::mat4 model;
    alignas(64) glm::mat4 view;
    alignas(64) glm::mat4 projection;
    alignas(64) glm::mat4 invModel;
} uniform;

struct Camera {
    float speed = 20;
    glm::vec3 eye = glm::vec3(0, 0, 3);
    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec2 angle;
    bool isDragging;

    void positionCameraCenter() {
        glm::vec3 direction;
        direction.x = cos(angle.y * PI / 180.0) * cos(angle.x * PI / 180.0);
        direction.y = sin(angle.x * PI / 180.0);
        direction.z = sin(angle.y * PI / 180.0) * cos(angle.x * PI / 180.0);
        center = eye + glm::normalize(direction);
    }
} camera;

glm::vec2 lastMousePosition;
glm::vec3 worldLightPosition;
float mouseSensitivity = 0.5;

void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods) {
    float moveSpeed = camera.speed * 0.01f;

    glm::vec3 forward = glm::normalize(camera.center - camera.eye);
    glm::vec3 right = glm::normalize(glm::cross(forward, camera.up));
    if (key == GLFW_KEY_W) {
        camera.eye = camera.eye + forward * moveSpeed;
        camera.center = camera.center + forward * moveSpeed;
        camera.up = glm::vec3(0, 1, 0);

    } else if (key == GLFW_KEY_S) {
        camera.eye = camera.eye + forward * -moveSpeed;
        camera.center = camera.center + forward * -moveSpeed;
        camera.up = glm::vec3(0, 1, 0);
    } else if (key == GLFW_KEY_D) {
        camera.eye = camera.eye + right * moveSpeed;
        camera.center = camera.center + right * moveSpeed;
        camera.up = glm::vec3(0, 1, 0);

    } else if (key == GLFW_KEY_A) {
        camera.eye = camera.eye + right * -moveSpeed;
        camera.center = camera.center + right * -moveSpeed;
        camera.up = glm::vec3(0, 1, 0);
    } else if (key == GLFW_KEY_Q) {
        camera.eye = camera.eye + glm::vec3(0, 1, 0) * moveSpeed;
        camera.center = camera.center + glm::vec3(0, 1, 0) * moveSpeed;
        camera.up = glm::vec3(0, 1, 0);

    } else if (key == GLFW_KEY_E) {
        camera.eye = camera.eye + glm::vec3(0, 1, 0) * -moveSpeed;
        camera.center = camera.center + glm::vec3(0, 1, 0) * -moveSpeed;
        camera.up = glm::vec3(0, 1, 0);
    }
}

void mouseButton(GLFWwindow *window, int button, int action, int modifier) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        camera.isDragging = true;
    } else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
        camera.isDragging = false;
    }
}

void mouseMovement(GLFWwindow *window, double xpos, double ypos) {
    ypos = HEIGHT - ypos;
    if (camera.isDragging) {

        float xDelta = (xpos - lastMousePosition.x);
        float yDelta = (ypos - lastMousePosition.y);
        float xOffset = xDelta * mouseSensitivity;
        float yOffset = yDelta * mouseSensitivity;

        camera.angle.x += yOffset;
        camera.angle.y += xOffset;

        if (camera.angle.x >= 89) {
            camera.angle.x = 89;
        } else if (camera.angle.x <= -89) {
            camera.angle.x = -89;
        }

        camera.positionCameraCenter();
    }
    lastMousePosition = glm::vec2(xpos, ypos);

}

std::vector<char> loadShader(const std::string &filename) {
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
    VkAttachmentDescription depthAttachment = vk::attachmentDescription(VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                                        VK_SAMPLE_COUNT_1_BIT,
                                                                        VK_ATTACHMENT_LOAD_OP_CLEAR,
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
                                                                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
                                                                   VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, 0,
                                                                   VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, 0);
    std::vector<VkAttachmentDescription> attachments = {colorAttachment, depthAttachment};
    std::vector<VkSubpassDependency> dependencies = {externalDependency};
    std::vector<VkSubpassDescription> subpasses = {colorSubpass};
    VkRenderPassCreateInfo defaultRenderPassCreateInfo = vk::renderPassCreateInfo(attachments, dependencies,
                                                                                  subpasses);
    return vk::createRenderPass(vkDevice, vk::renderPassCreateInfo(attachments, dependencies, subpasses));
}

void updateUniformBuffer(const VkDevice &device, const Model &model, RenderFrame frame) {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    ImGui::DragFloat3("Light Position", (float *) &worldLightPosition, 0.1f);
    ImGui::ColorPicker3("Light Color", (float *) &uniform.lightColor);
    ImGui::DragFloat("Light Intensity ", &uniform.lightColor.a, 0.1f);
    ImGui::Checkbox("Use normal mapping", &uniform.normalMapping);
    uniform.model =
            glm::translate(glm::mat4(1.0f), model.position()) *
            glm::rotate(glm::mat4(1.0f), time * glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    uniform.view = glm::lookAt(camera.eye, camera.center, camera.up);
    uniform.projection = glm::perspective(glm::radians(45.0f), (float) WIDTH / (float) HEIGHT, 0.1f, 10.0f);
    uniform.projection[1][1] *= -1;
    uniform.invModel = glm::inverse(uniform.model);

    /* Convert from world space to model space */
    uniform.viewPosition = uniform.invModel * glm::vec4(camera.eye, 1.0f);
    uniform.lightPosition = uniform.invModel * glm::vec4(worldLightPosition, 1.0f);

    void *data;
    vkMapMemory(device, frame.uniformBuffer.memory.vkDeviceMemory, 0, sizeof(uniform), 0, &data);
    memcpy(data, &uniform, sizeof(uniform));
    vkUnmapMemory(device, frame.uniformBuffer.memory.vkDeviceMemory);
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

    //const Model& model = Human();
    const Model &model = DamagedHelmet();

    VMA::getInstance().initialize(vkDevice, physicalDeviceInfo, new PassThroughAllocator());
    vk::Queue graphicsQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.graphicsFamily, 0);
    vk::Queue computeQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.computeFamily, 1);
    vk::Queue presentationQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.presentationFamily, 0);
    vk::Queue transferQueue = vk::Queue(vkDevice, physicalDeviceInfo.queueFamilies.transferFamily, 0);

    uint32_t renderFramesAmount = 2;

    ResourcesManager resourcesManager(vkDevice);

    resourcesManager.addUniformBufferLayout(renderFramesAmount);
    for (int i = 0; i < model.textures().size(); i++) {
        resourcesManager.addTextureSamplerLayout(1);
    }

    vk::DescriptorSetLayout descriptorSetLayout = vk::DescriptorSetLayout(vkDevice)
            .addBinding("tex_albedo", VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
            .addBinding("tex_normal", VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
            .addBinding("tex_metallic", VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT)
            .build();
    auto descriptorSetLayouts = resourcesManager.commitDescriptorBindings();

    VkPipelineLayout pipelineLayout;
    std::vector<uint32_t> graphicsQueueList = {graphicsQueue.queueIndex};
    VkImage depthBuffer = vk::createImage(vkDevice,
                                          vk::imageCreateInfo(0, VK_IMAGE_TYPE_2D, VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                              vk::extent3D(presentationEngineInfo.extents.width, presentationEngineInfo.extents.height, 1), 1, 1,
                                                              VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                                                              VK_SHARING_MODE_EXCLUSIVE, graphicsQueueList, VK_IMAGE_LAYOUT_UNDEFINED));
    VkMemoryRequirements requirements = vk::getImageMemoryRequirements(vkDevice, depthBuffer);
    AllocationBlock memory = VMA::getInstance().vmalloc(requirements, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkBindImageMemory(vkDevice, depthBuffer, memory.vkDeviceMemory, memory.vkOffset);
    VkImageView depthBufferView = vk::createImageView(vkDevice, vk::imageViewCreateInfo(depthBuffer, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_D32_SFLOAT_S8_UINT,
                                                                                        {VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY},
                                                                                        vk::imageSubresourceRange(VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1)));
    VkRenderPass defaultRenderPass = createDefaultRenderPass(vkDevice, presentationEngineInfo);
    VkRenderPass imGuiRenderPass = VulkanGui::ImGuiCreateRenderPass(vkDevice, presentationEngineInfo);
    // Pipeline Creation
    {
        auto bindingDescriptions = PnuVertexInput::getInputBindingDescription();
        auto attributeDescriptions = PnuVertexInput::getInputAttributeDescription();

        VkPipelineVertexInputStateCreateInfo vertexInputState = vk::pipelineVertexInputStateCreateInfo(bindingDescriptions, attributeDescriptions);

        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = vk::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
        VkPipelineRasterizationStateCreateInfo rasterizationState = vk::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT,
                                                                                                             VK_FRONT_FACE_COUNTER_CLOCKWISE, 1.0);
        VkPipelineDepthStencilStateCreateInfo depthStencilState = vk::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
        VkPipelineTessellationStateCreateInfo tesselationState = vk::pipelineTessellationStateCreateInfo(0);

        VkPipelineColorBlendAttachmentState colorBlendAttachmentState = vk::pipelineColorBlendAttachmentState();
        colorBlendAttachmentState.blendEnable = VK_FALSE;
        colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_R_BIT |
                                                   VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;

        VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = vk::pipelineLayoutCreateInfo(descriptorSetLayouts, {});
        vkCreatePipelineLayout(vkDevice, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout);
        float blendConstants[4] = {1, 1, 1, 1};
        std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments = {colorBlendAttachmentState};
        VkPipelineColorBlendStateCreateInfo colorBlendState = vk::pipelineColorBlendStateCreateInfo(colorBlendAttachments, blendConstants, VK_FALSE, VK_LOGIC_OP_NO_OP);
        auto vertexBytes = loadShader(FileLoader::getPath("Shaders/Compiled/" + model.shader() + ".vert.spv"));
        auto fragmentBytes = loadShader(FileLoader::getPath("Shaders/Compiled/" + model.shader() + ".frag.spv"));
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


    VkCommandBuffer *graphicsCommandBuffers = {vk::allocateCommandBuffers(vkDevice, vk::commandBufferAllocateInfo(graphicsPool, renderFramesAmount, VK_COMMAND_BUFFER_LEVEL_PRIMARY),
                                                                          renderFramesAmount)};

    std::vector<uint32_t> graphicsAndTransferQueues = {physicalDeviceInfo.queueFamilies.graphicsFamily, physicalDeviceInfo.queueFamilies.transferFamily};

    std::vector<RenderFrame> renderFrames(renderFramesAmount);

    for (int i = 0; i < renderFramesAmount; ++i) {
        RenderFrame renderFrame{};
        renderFrame.commandBuffer = graphicsCommandBuffers[i];
        renderFrame.imageReadySemaphore = vk::createSemaphore(vkDevice, vk::semaphoreCreateInfo());
        renderFrame.presentationReadySemaphore = vk::createSemaphore(vkDevice, vk::semaphoreCreateInfo());
        renderFrame.bufferFinishedFence = vk::createFence(vkDevice, vk::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT));
        renderFrame.frameBuffer = VK_NULL_HANDLE;
        std::vector<uint32_t> queues{graphicsQueue.queueFamily};
        renderFrame.uniformBuffer = vk::Buffer(vkDevice, (VkDeviceSize) sizeof(Uniform), graphicsAndTransferQueues, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE, 0,
                                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        renderFrames[i] = renderFrame;
    }
    VulkanGui::ImGuiSetupForVulkan(window, vulkanSetup, 0, graphicsQueue.queue, imGuiRenderPass, graphicsPool);

    vk::HostDeviceTransfer hostDeviceTransfer = vk::HostDeviceTransfer(vkDevice, transferQueue);

    std::vector<PnuVertexInput> vertexInputs;
    std::vector<uint32_t> indexVector;
    ObjectLoader::loadPnuModel(model.meshPath(), vertexInputs, indexVector);
    ObjectLoader::calculateTangents(vertexInputs, indexVector);
    vk::Buffer vertexBuffer = vk::Buffer(vkDevice, sizeof(PnuVertexInput) * vertexInputs.size(), graphicsAndTransferQueues,
                                         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_SHARING_MODE_CONCURRENT, 0,
                                         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    hostDeviceTransfer.transferBuffer(vertexBuffer.size, vertexInputs.data(), vertexBuffer);
    vk::Buffer indexBuffer = vk::Buffer(vkDevice, indexVector.size() * sizeof(uint32_t), graphicsAndTransferQueues,
                                        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                                        VK_SHARING_MODE_CONCURRENT, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    hostDeviceTransfer.transferBuffer(indexBuffer.size, indexVector.data(), indexBuffer);

    vk::CommandBuffer commandBuffer = vk::CommandBuffer(vkDevice, graphicsQueue, graphicsPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    std::vector<Texture> textures;
    for (const auto &texture : model.textures()) {
        textures.emplace_back(vkDevice, texture, hostDeviceTransfer, graphicsAndTransferQueues, commandBuffer);
    }

    auto descriptorPool = resourcesManager.createDescriptorPools();

    // Todo: Move to Resources Manager
    VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo = vk::descriptorSetAllocateInfo(descriptorPool, descriptorSetLayouts);
    VkDescriptorSet descriptorSet = vk::allocateDescriptorSets(vkDevice, renderFramesAmount + 1, &vkDescriptorSetAllocateInfo)[0];

    for (auto &renderFrame : renderFrames) {
        std::vector<VkWriteDescriptorSet> descriptorWriteSet{};

        /* Uniform Buffers */
        VkDescriptorBufferInfo vkUniformBufferInfo = vk::descriptorBufferInfo(renderFrame.uniformBuffer.buffer, 0,
                                                                              renderFrame.uniformBuffer.size);
        std::vector<VkDescriptorBufferInfo> descriptorBufferInfoVector = {vkUniformBufferInfo};
        VkWriteDescriptorSet uniformDescriptorSetWrite = vk::writeDescriptorSet(descriptorBufferInfoVector, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorSet, 0, 0);
        descriptorWriteSet.push_back(uniformDescriptorSetWrite);

        vkUpdateDescriptorSets(vkDevice, descriptorWriteSet.size(), descriptorWriteSet.data(), 0, nullptr);
    }

    uint32_t binding = 1;

    std::vector<VkWriteDescriptorSet> descriptorSets;
    for (auto &texture : textures) {
        descriptorSets.push_back(texture.getWriteDescriptorSet(binding++, descriptorSet));
    }

    vkUpdateDescriptorSets(vkDevice, descriptorSets.size(), descriptorSets.data(), 0, nullptr);


    VkClearValue colorBufferClearValue;
    VkClearValue depthBufferClearValue;
    colorBufferClearValue.color = {0.04f, 0.04f, 0.04f, 1};
    depthBufferClearValue.depthStencil = {1, 0};
    std::vector<VkClearValue> clearValues = {colorBufferClearValue, depthBufferClearValue};
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
            updateUniformBuffer(vkDevice, model, currentFrame);
            ImGui::Render();

            std::vector<VkImageView> framebufferAttachments = {swapchainImageViews[swapchainImageIndex], depthBufferView};
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
                    vkCmdBindDescriptorSets(currentFrame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
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