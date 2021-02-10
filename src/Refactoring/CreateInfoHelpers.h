//
// Created by menegais on 16/01/2021.
//

#ifndef VULKANBASE_CREATEINFOHELPERS_H
#define VULKANBASE_CREATEINFOHELPERS_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

namespace vk {

    inline VkBufferCreateInfo bufferCreateInfo(VkDeviceSize size, const std::vector<uint32_t> &queueFamilyIndices,
                                               VkBufferUsageFlags usage, VkSharingMode sharingMode, VkBufferCreateFlags flags) {
        VkBufferCreateInfo vkBufferCreateInfo{};
        vkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        vkBufferCreateInfo.size = size;
        vkBufferCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        vkBufferCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
        vkBufferCreateInfo.usage = usage;
        vkBufferCreateInfo.sharingMode = sharingMode;
        vkBufferCreateInfo.flags = flags;
        return vkBufferCreateInfo;
    }

    inline VkMemoryAllocateInfo memoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex) {
        VkMemoryAllocateInfo vkMemoryAllocateInfo{};
        vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        vkMemoryAllocateInfo.allocationSize = allocationSize;
        vkMemoryAllocateInfo.memoryTypeIndex = memoryTypeIndex;
        return vkMemoryAllocateInfo;
    }

    inline VkMappedMemoryRange mappedMemoryRange(VkDeviceMemory memory, VkDeviceSize offset, VkDeviceSize size) {
        VkMappedMemoryRange vkMappedMemoryRange{};
        vkMappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        vkMappedMemoryRange.memory = memory;
        vkMappedMemoryRange.offset = offset;
        vkMappedMemoryRange.size = size;
        return vkMappedMemoryRange;
    }

    inline VkInstanceCreateInfo instanceCreateInfo(const std::vector<const char *> &extensions, const std::vector<const char *> &layers, VkApplicationInfo *applicationInfo) {
        VkInstanceCreateInfo vkInstanceCreateInfo{};
        vkInstanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        vkInstanceCreateInfo.enabledExtensionCount = extensions.size();
        vkInstanceCreateInfo.ppEnabledExtensionNames = extensions.data();
        vkInstanceCreateInfo.enabledLayerCount = layers.size();
        vkInstanceCreateInfo.ppEnabledLayerNames = layers.data();
        vkInstanceCreateInfo.pApplicationInfo = applicationInfo;
        vkInstanceCreateInfo.flags = 0;
        return vkInstanceCreateInfo;

    }

    inline VkApplicationInfo applicationInfo(uint32_t apiVersion, uint32_t engineVersion, uint32_t applicationVersion, const char *engineName, const char *applicationName) {
        VkApplicationInfo vkApplicationInfo{};
        vkApplicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        vkApplicationInfo.apiVersion = apiVersion;
        vkApplicationInfo.pApplicationName = applicationName;
        vkApplicationInfo.pEngineName = engineName;
        vkApplicationInfo.applicationVersion = applicationVersion;
        vkApplicationInfo.engineVersion = engineVersion;
        return vkApplicationInfo;
    }

    inline VkDeviceCreateInfo
    deviceCreateInfo(const std::vector<const char *> &extensions, const std::vector<const char *> &layers,
                     const std::vector<VkPhysicalDeviceFeatures> &features, const std::vector<VkDeviceQueueCreateInfo> &queuesCreateInfo) {
        VkDeviceCreateInfo vkDeviceCreateInfo{};
        vkDeviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        vkDeviceCreateInfo.enabledExtensionCount = extensions.size();
        vkDeviceCreateInfo.ppEnabledExtensionNames = extensions.data();
        vkDeviceCreateInfo.enabledLayerCount = layers.size();
        vkDeviceCreateInfo.ppEnabledLayerNames = layers.data();
        vkDeviceCreateInfo.pEnabledFeatures = features.data();
        vkDeviceCreateInfo.queueCreateInfoCount = queuesCreateInfo.size();
        vkDeviceCreateInfo.pQueueCreateInfos = queuesCreateInfo.data();

        return vkDeviceCreateInfo;
    }

    inline VkDeviceQueueCreateInfo queueCreateInfo(uint32_t queueFamilyIndex, uint32_t queueCount, const float *queuePriorites) {
        VkDeviceQueueCreateInfo vkDeviceQueueCreateInfo{};
        vkDeviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        vkDeviceQueueCreateInfo.queueFamilyIndex = queueFamilyIndex;
        vkDeviceQueueCreateInfo.queueCount = queueCount;
        vkDeviceQueueCreateInfo.pQueuePriorities = queuePriorites;
        return vkDeviceQueueCreateInfo;
    }

    inline VkSwapchainCreateInfoKHR swapchainCreateInfoKhr(VkSurfaceKHR surface, VkPresentModeKHR presentMode,
                                                           VkFormat imageFormat, VkColorSpaceKHR imageColorSpace,
                                                           uint32_t minImageCount, VkExtent2D imageExtent, const std::vector<uint32_t> &queueFamilyIndices) {
        VkSwapchainCreateInfoKHR vkSwapchainCreateInfoKhr{};
        vkSwapchainCreateInfoKhr.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        vkSwapchainCreateInfoKhr.oldSwapchain = VK_NULL_HANDLE;
        vkSwapchainCreateInfoKhr.surface = surface;
        vkSwapchainCreateInfoKhr.presentMode = presentMode;
        vkSwapchainCreateInfoKhr.imageFormat = imageFormat;
        vkSwapchainCreateInfoKhr.imageColorSpace = imageColorSpace;
        vkSwapchainCreateInfoKhr.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
        vkSwapchainCreateInfoKhr.minImageCount = minImageCount;
        vkSwapchainCreateInfoKhr.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        vkSwapchainCreateInfoKhr.imageArrayLayers = 1;
        vkSwapchainCreateInfoKhr.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        vkSwapchainCreateInfoKhr.clipped = VK_TRUE;
        vkSwapchainCreateInfoKhr.imageExtent = imageExtent;
        vkSwapchainCreateInfoKhr.queueFamilyIndexCount = queueFamilyIndices.size();
        vkSwapchainCreateInfoKhr.pQueueFamilyIndices = queueFamilyIndices.data();
        vkSwapchainCreateInfoKhr.imageSharingMode = queueFamilyIndices.size() == 1 ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT;
        return vkSwapchainCreateInfoKhr;
    }

    inline VkSemaphoreCreateInfo semaphoreCreateInfo() {
        VkSemaphoreCreateInfo vkSemaphoreCreateInfo{};
        vkSemaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        return vkSemaphoreCreateInfo;
    }

    inline VkFenceCreateInfo fenceCreateInfo(VkFenceCreateFlags flags) {
        VkFenceCreateInfo vkFenceCreateInfo{};
        vkFenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        vkFenceCreateInfo.flags = flags;
        return vkFenceCreateInfo;
    }

    inline VkVertexInputBindingDescription vertexInputBindingDescription(uint32_t binding, uint32_t stride, VkVertexInputRate inputRate) {
        VkVertexInputBindingDescription vkVertexInputBindingDescription{};
        vkVertexInputBindingDescription.binding = binding;
        vkVertexInputBindingDescription.stride = stride;
        vkVertexInputBindingDescription.inputRate = inputRate;
        return vkVertexInputBindingDescription;
    }

    inline VkVertexInputAttributeDescription vertexInputAttributeDescription(uint32_t binding, VkFormat format, uint32_t location, uint32_t offset) {
        VkVertexInputAttributeDescription vkVertexInputAttributeDescription{};
        vkVertexInputAttributeDescription.binding = binding;
        vkVertexInputAttributeDescription.format = format;
        vkVertexInputAttributeDescription.location = location;
        vkVertexInputAttributeDescription.offset = offset;
        return vkVertexInputAttributeDescription;
    }

    inline VkPipelineInputAssemblyStateCreateInfo pipelineInputAssemblyStateCreateInfo(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable) {
        VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo{};
        vkPipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        vkPipelineInputAssemblyStateCreateInfo.topology = topology;
        vkPipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestartEnable;
        return vkPipelineInputAssemblyStateCreateInfo;
    }

    inline VkPipelineVertexInputStateCreateInfo
    pipelineVertexInputStateCreateInfo(const std::vector<VkVertexInputBindingDescription> &vertexAttributeBindingDescriptions, const std::vector<VkVertexInputAttributeDescription> &vertexAttributeDescriptions) {
        VkPipelineVertexInputStateCreateInfo vkPipelineVertexInputStateCreateInfo{};
        vkPipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vkPipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vertexAttributeBindingDescriptions.size();
        vkPipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vertexAttributeDescriptions.data();
        vkPipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vertexAttributeDescriptions.size();
        vkPipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vertexAttributeBindingDescriptions.data();
        return vkPipelineVertexInputStateCreateInfo;
    }

    inline VkPipelineTessellationStateCreateInfo pipelineTessellationStateCreateInfo(uint32_t patchControlPoints) {
        VkPipelineTessellationStateCreateInfo vkPipelineTessellationStateCreateInfo{};
        vkPipelineTessellationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
        vkPipelineTessellationStateCreateInfo.patchControlPoints = patchControlPoints;
        return vkPipelineTessellationStateCreateInfo;
    }

    inline VkViewport viewport(float x, float y, float width, float height, float minDepth, float maxDepth) {
        VkViewport vkViewport{};
        vkViewport.x = x;
        vkViewport.y = y;
        vkViewport.width = width;
        vkViewport.height = height;
        vkViewport.minDepth = minDepth;
        vkViewport.maxDepth = maxDepth;
        return vkViewport;
    }

    inline VkRect2D rect2D(VkExtent2D extent, VkOffset2D offset) {
        VkRect2D vkRect2D{};
        vkRect2D.extent = extent;
        vkRect2D.offset = offset;
        return vkRect2D;
    }

    inline VkExtent2D extent2D(uint32_t width, uint32_t height) {
        VkExtent2D vkExtent2D{};
        vkExtent2D.width = width;
        vkExtent2D.height = height;
        return vkExtent2D;
    }

    inline VkExtent3D extent3D(uint32_t width, uint32_t height, uint32_t depth) {
        VkExtent3D vkExtent3D{};
        vkExtent3D.width = width;
        vkExtent3D.height = height;
        vkExtent3D.depth = depth;
        return vkExtent3D;
    }

    inline VkOffset2D offset2D(int32_t x, int32_t y) {
        VkOffset2D vkOffset2D{};
        vkOffset2D.x = x;
        vkOffset2D.y = y;
        return vkOffset2D;
    }

    inline VkOffset3D offset3D(int32_t x, int32_t y, int32_t z) {
        VkOffset3D vkOffset3D{};
        vkOffset3D.x = x;
        vkOffset3D.y = y;
        vkOffset3D.z = z;
        return vkOffset3D;
    }

    inline VkPipelineViewportStateCreateInfo pipelineViewportStateCreateInfo(const std::vector<VkViewport> &viewports, const std::vector<VkRect2D> &scissors) {
        VkPipelineViewportStateCreateInfo vkPipelineViewportStateCreateInfo{};
        vkPipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        vkPipelineViewportStateCreateInfo.scissorCount = scissors.size();
        vkPipelineViewportStateCreateInfo.pScissors = scissors.data();
        vkPipelineViewportStateCreateInfo.viewportCount = viewports.size();
        vkPipelineViewportStateCreateInfo.pViewports = viewports.data();
        return vkPipelineViewportStateCreateInfo;
    }

    inline VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo(VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, float lineWidth) {
        VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo{};
        vkPipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        vkPipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
        vkPipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        vkPipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
        vkPipelineRasterizationStateCreateInfo.depthBiasClamp = 0;
        vkPipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0;
        vkPipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0;
        vkPipelineRasterizationStateCreateInfo.frontFace = frontFace;
        vkPipelineRasterizationStateCreateInfo.cullMode = cullMode;
        vkPipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
        vkPipelineRasterizationStateCreateInfo.lineWidth = lineWidth;
        return vkPipelineRasterizationStateCreateInfo;
    }

    inline VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo(VkSampleCountFlagBits rasterizationSamples, VkBool32 sampleShadingEnable,
                                                                                   float minSampleShading, const VkSampleMask *pSampleMask,
                                                                                   VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable) {
        VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo{};
        vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        vkPipelineMultisampleStateCreateInfo.sampleShadingEnable = sampleShadingEnable;
        vkPipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
        vkPipelineMultisampleStateCreateInfo.minSampleShading = minSampleShading;
        vkPipelineMultisampleStateCreateInfo.pSampleMask = pSampleMask;
        vkPipelineMultisampleStateCreateInfo.alphaToCoverageEnable = alphaToCoverageEnable;
        vkPipelineMultisampleStateCreateInfo.alphaToOneEnable = alphaToOneEnable;
        return vkPipelineMultisampleStateCreateInfo;
    }

    inline VkPipelineMultisampleStateCreateInfo pipelineMultisampleStateCreateInfo() {
        VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo{};
        vkPipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        return vkPipelineMultisampleStateCreateInfo;
    }


    inline VkPipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo(const std::vector<VkPipelineColorBlendAttachmentState> &attachments, float blendConstants[4],
                                                                                 VkBool32 logicOpEnable, VkLogicOp logicOp) {
        VkPipelineColorBlendStateCreateInfo vkPipelineColorBlendStateCreateInfo{};
        vkPipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        vkPipelineColorBlendStateCreateInfo.attachmentCount = attachments.size();
        vkPipelineColorBlendStateCreateInfo.pAttachments = attachments.data();
        vkPipelineColorBlendStateCreateInfo.blendConstants[0] = blendConstants[0];
        vkPipelineColorBlendStateCreateInfo.blendConstants[1] = blendConstants[1];
        vkPipelineColorBlendStateCreateInfo.blendConstants[2] = blendConstants[2];
        vkPipelineColorBlendStateCreateInfo.blendConstants[3] = blendConstants[3];
        vkPipelineColorBlendStateCreateInfo.logicOpEnable = logicOpEnable;
        vkPipelineColorBlendStateCreateInfo.logicOp = logicOp;
        return vkPipelineColorBlendStateCreateInfo;
    }

    inline VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState(VkBool32 blendEnable, VkBlendFactor srcColorBlendFactor, VkBlendFactor dstColorBlendFactor,
                                                                                 VkBlendOp colorBlendOp, VkBlendFactor srcAlphaBlendFactor,
                                                                                 VkBlendFactor dstAlphaBlendFactor,
                                                                                 VkBlendOp alphaBlendOp,
                                                                                 VkColorComponentFlags colorWriteMask) {
        VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState{};
        vkPipelineColorBlendAttachmentState.blendEnable = blendEnable;
        vkPipelineColorBlendAttachmentState.alphaBlendOp = alphaBlendOp;
        vkPipelineColorBlendAttachmentState.colorBlendOp = colorBlendOp;
        vkPipelineColorBlendAttachmentState.colorWriteMask = colorWriteMask;
        vkPipelineColorBlendAttachmentState.dstAlphaBlendFactor = dstAlphaBlendFactor;
        vkPipelineColorBlendAttachmentState.srcAlphaBlendFactor = srcAlphaBlendFactor;
        vkPipelineColorBlendAttachmentState.dstColorBlendFactor = dstColorBlendFactor;
        vkPipelineColorBlendAttachmentState.srcColorBlendFactor = srcColorBlendFactor;
        return vkPipelineColorBlendAttachmentState;
    }

    inline VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState() {
        VkPipelineColorBlendAttachmentState vkPipelineColorBlendAttachmentState{};
        return vkPipelineColorBlendAttachmentState;
    }

    inline VkPipelineDepthStencilStateCreateInfo pipelineDepthStencilStateCreateInfo(VkBool32 depthTestEnable, VkBool32 depthTestWriteEnable, VkCompareOp depthCompareOp) {
        VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo{};
        vkPipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        vkPipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
        vkPipelineDepthStencilStateCreateInfo.depthWriteEnable = depthTestWriteEnable;
        vkPipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
        return vkPipelineDepthStencilStateCreateInfo;
    }

    inline VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo(const std::vector<VkDescriptorSetLayout> &setLayouts, const std::vector<VkPushConstantRange> &pushConstantRanges) {
        VkPipelineLayoutCreateInfo vkPipelineLayoutCreateInfo{};
        vkPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        vkPipelineLayoutCreateInfo.setLayoutCount = setLayouts.size();
        vkPipelineLayoutCreateInfo.pSetLayouts = setLayouts.data();
        vkPipelineLayoutCreateInfo.pushConstantRangeCount = pushConstantRanges.size();
        vkPipelineLayoutCreateInfo.pPushConstantRanges = pushConstantRanges.data();
        return vkPipelineLayoutCreateInfo;
    }

    inline VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo(VkShaderModule module, char *name, VkShaderStageFlagBits stage) {
        VkPipelineShaderStageCreateInfo vkPipelineShaderStageCreateInfo{};
        vkPipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vkPipelineShaderStageCreateInfo.module = module;
        vkPipelineShaderStageCreateInfo.pName = name;
        vkPipelineShaderStageCreateInfo.stage = stage;
        return vkPipelineShaderStageCreateInfo;
    }

    inline VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo(const std::vector<VkPipelineShaderStageCreateInfo> &stages, const VkPipelineVertexInputStateCreateInfo *vertexInputState,
                                                                   const VkPipelineInputAssemblyStateCreateInfo *inputAssemblyState, const VkPipelineTessellationStateCreateInfo *tessellationState,
                                                                   const VkPipelineViewportStateCreateInfo *viewportState, const VkPipelineRasterizationStateCreateInfo *rasterizationState,
                                                                   const VkPipelineMultisampleStateCreateInfo *multisampleState, const VkPipelineDepthStencilStateCreateInfo *depthStencilState,
                                                                   const VkPipelineColorBlendStateCreateInfo *colorBlendState, const VkPipelineDynamicStateCreateInfo *dynamicState,
                                                                   VkPipelineLayout layout, VkRenderPass renderPass, uint32_t subpass) {
        VkGraphicsPipelineCreateInfo vkGraphicsPipelineCreateInfo{};
        vkGraphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        vkGraphicsPipelineCreateInfo.renderPass = renderPass;
        vkGraphicsPipelineCreateInfo.subpass = subpass;
        vkGraphicsPipelineCreateInfo.stageCount = stages.size();
        vkGraphicsPipelineCreateInfo.pStages = stages.data();
        vkGraphicsPipelineCreateInfo.pColorBlendState = colorBlendState;
        vkGraphicsPipelineCreateInfo.pVertexInputState = vertexInputState;
        vkGraphicsPipelineCreateInfo.pInputAssemblyState = inputAssemblyState;
        vkGraphicsPipelineCreateInfo.pMultisampleState = multisampleState;
        vkGraphicsPipelineCreateInfo.pViewportState = viewportState;
        vkGraphicsPipelineCreateInfo.pRasterizationState = rasterizationState;
        vkGraphicsPipelineCreateInfo.pTessellationState = tessellationState;
        vkGraphicsPipelineCreateInfo.pDepthStencilState = depthStencilState;
        vkGraphicsPipelineCreateInfo.pDynamicState = dynamicState;
        vkGraphicsPipelineCreateInfo.layout = layout;
        return vkGraphicsPipelineCreateInfo;
    }

    inline VkDescriptorSetAllocateInfo descriptorSetAllocateInfo(VkDescriptorPool descriptorPool, const std::vector<VkDescriptorSetLayout> &setLayouts) {
        VkDescriptorSetAllocateInfo vkDescriptorSetAllocateInfo{};
        vkDescriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        vkDescriptorSetAllocateInfo.descriptorPool = descriptorPool;
        vkDescriptorSetAllocateInfo.descriptorSetCount = setLayouts.size();
        vkDescriptorSetAllocateInfo.pSetLayouts = setLayouts.data();
        return vkDescriptorSetAllocateInfo;
    }

    inline VkDescriptorPoolSize descriptorPoolSize(VkDescriptorType type, uint32_t descriptorCount) {
        VkDescriptorPoolSize vkDescriptorPoolSize{};
        vkDescriptorPoolSize.type = type;
        vkDescriptorPoolSize.descriptorCount = descriptorCount;
        return vkDescriptorPoolSize;
    }

    inline VkDescriptorPoolCreateInfo descriptorPoolCreateInfo(const std::vector<VkDescriptorPoolSize> &poolSizes, uint32_t maxSets) {
        VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo{};
        vkDescriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        vkDescriptorPoolCreateInfo.poolSizeCount = poolSizes.size();
        vkDescriptorPoolCreateInfo.pPoolSizes = poolSizes.data();
        vkDescriptorPoolCreateInfo.maxSets = maxSets;
        return vkDescriptorPoolCreateInfo;
    }


    inline VkDescriptorSetLayoutBinding descriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, uint32_t descriptorCount, VkShaderStageFlags stageFlags) {
        VkDescriptorSetLayoutBinding vkDescriptorSetLayoutBinding{};
        vkDescriptorSetLayoutBinding.binding = binding;
        vkDescriptorSetLayoutBinding.descriptorCount = descriptorCount;
        vkDescriptorSetLayoutBinding.descriptorType = descriptorType;
        vkDescriptorSetLayoutBinding.stageFlags = stageFlags;
        return vkDescriptorSetLayoutBinding;
    }

    inline VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo(const std::vector<VkDescriptorSetLayoutBinding> &bindings) {
        VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutInfo{};
        vkDescriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        vkDescriptorSetLayoutInfo.bindingCount = bindings.size();
        vkDescriptorSetLayoutInfo.pBindings = bindings.data();
        return vkDescriptorSetLayoutInfo;
    }

    inline VkWriteDescriptorSet writeDescriptorSet(const std::vector<VkDescriptorBufferInfo> &bufferInfo, VkDescriptorType descriptorType, VkDescriptorSet descriptorSet,
                                                   uint32_t dstBinding, uint32_t dstArrayElement) {
        VkWriteDescriptorSet vkWriteDescriptorSet{};
        vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vkWriteDescriptorSet.descriptorCount = bufferInfo.size();
        vkWriteDescriptorSet.descriptorType = descriptorType;
        vkWriteDescriptorSet.dstSet = descriptorSet;
        vkWriteDescriptorSet.dstBinding = dstBinding;
        vkWriteDescriptorSet.dstArrayElement = dstArrayElement;
        vkWriteDescriptorSet.pBufferInfo = bufferInfo.data();
        return vkWriteDescriptorSet;
    }

    inline VkWriteDescriptorSet writeDescriptorSet(const std::vector<VkDescriptorImageInfo> &imageInfo, VkDescriptorType descriptorType, VkDescriptorSet descriptorSet,
                                                   uint32_t dstBinding, uint32_t dstArrayElement) {
        VkWriteDescriptorSet vkWriteDescriptorSet{};
        vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vkWriteDescriptorSet.descriptorCount = imageInfo.size();
        vkWriteDescriptorSet.descriptorType = descriptorType;
        vkWriteDescriptorSet.dstSet = descriptorSet;
        vkWriteDescriptorSet.dstBinding = dstBinding;
        vkWriteDescriptorSet.dstArrayElement = dstArrayElement;
        vkWriteDescriptorSet.pImageInfo = imageInfo.data();
        return vkWriteDescriptorSet;
    }

    inline VkWriteDescriptorSet writeDescriptorSet(const std::vector<VkBufferView> &texelBufferView, VkDescriptorType descriptorType, VkDescriptorSet descriptorSet,
                                                   uint32_t dstBinding, uint32_t dstArrayElement) {
        VkWriteDescriptorSet vkWriteDescriptorSet{};
        vkWriteDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        vkWriteDescriptorSet.descriptorCount = texelBufferView.size();
        vkWriteDescriptorSet.descriptorType = descriptorType;
        vkWriteDescriptorSet.dstSet = descriptorSet;
        vkWriteDescriptorSet.dstBinding = dstBinding;
        vkWriteDescriptorSet.dstArrayElement = dstArrayElement;
        vkWriteDescriptorSet.pTexelBufferView = texelBufferView.data();
        return vkWriteDescriptorSet;
    }

    inline VkCommandPoolCreateInfo commandPoolCreateInfo(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags) {
        VkCommandPoolCreateInfo vkCommandPoolCreateInfo{};
        vkCommandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        vkCommandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
        vkCommandPoolCreateInfo.flags = flags;
        return vkCommandPoolCreateInfo;
    }

    inline VkCommandBufferAllocateInfo commandBufferAllocateInfo(VkCommandPool vkCommandPool, uint32_t commandBufferCount, VkCommandBufferLevel level) {
        VkCommandBufferAllocateInfo vkCommandBufferAllocateInfo{};
        vkCommandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        vkCommandBufferAllocateInfo.commandPool = vkCommandPool;
        vkCommandBufferAllocateInfo.commandBufferCount = commandBufferCount;
        vkCommandBufferAllocateInfo.level = level;
        return vkCommandBufferAllocateInfo;
    }

    inline VkSubmitInfo submitInfo(const std::vector<VkCommandBuffer> &commandBuffers, const std::vector<VkSemaphore> &waitSemaphores,
                                   const std::vector<VkSemaphore> &signalSemaphores, VkPipelineStageFlags *pWaitDstStageMask = nullptr) {
        VkSubmitInfo vkSubmitInfo{};
        vkSubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        vkSubmitInfo.commandBufferCount = commandBuffers.size();
        vkSubmitInfo.pCommandBuffers = commandBuffers.data();
        vkSubmitInfo.waitSemaphoreCount = waitSemaphores.size();
        vkSubmitInfo.signalSemaphoreCount = signalSemaphores.size();
        vkSubmitInfo.pSignalSemaphores = signalSemaphores.data();
        vkSubmitInfo.pWaitSemaphores = waitSemaphores.data();
        vkSubmitInfo.pWaitDstStageMask = pWaitDstStageMask;
        return vkSubmitInfo;
    }

    inline VkAttachmentDescription attachmentDescription(VkFormat format, VkSampleCountFlagBits samples,
                                                         VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp,
                                                         VkImageLayout initialLayout, VkImageLayout finalLayout,
                                                         VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                                         VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE) {
        VkAttachmentDescription attachmentDescription{};
        attachmentDescription.samples = samples;
        attachmentDescription.format = format;
        attachmentDescription.initialLayout = initialLayout;
        attachmentDescription.finalLayout = finalLayout;
        attachmentDescription.loadOp = loadOp;
        attachmentDescription.storeOp = storeOp;
        attachmentDescription.stencilLoadOp = stencilLoadOp;
        attachmentDescription.stencilStoreOp = stencilStoreOp;
        return attachmentDescription;
    }

    inline VkAttachmentReference attachmentReference(uint32_t attachment, VkImageLayout layout) {
        VkAttachmentReference attachmentReference{};
        attachmentReference.attachment = attachment;
        attachmentReference.layout = layout;
        return attachmentReference;
    }

    inline VkSubpassDescription subpassDescription(VkPipelineBindPoint pipelineBindPoint,
                                                   const std::vector<VkAttachmentReference> &colorAttachments, const std::vector<VkAttachmentReference> &inputAttachments,
                                                   const std::vector<VkAttachmentReference> &resolveAttachments, const std::vector<VkAttachmentReference> &depthStencilAttachments,
                                                   const std::vector<uint32_t> &preserveAttachments) {
        VkSubpassDescription vkSubpassDescription{};
        vkSubpassDescription.pipelineBindPoint = pipelineBindPoint;
        vkSubpassDescription.inputAttachmentCount = inputAttachments.size();
        vkSubpassDescription.pInputAttachments = inputAttachments.data();
        vkSubpassDescription.colorAttachmentCount = colorAttachments.size();
        vkSubpassDescription.pColorAttachments = colorAttachments.data();
        vkSubpassDescription.pResolveAttachments = resolveAttachments.data();
        vkSubpassDescription.pDepthStencilAttachment = depthStencilAttachments.data();
        vkSubpassDescription.preserveAttachmentCount = preserveAttachments.size();
        vkSubpassDescription.pPreserveAttachments = preserveAttachments.data();
        return vkSubpassDescription;
    }

    inline VkSubpassDependency subpassDependency(uint32_t srcSubpass, uint32_t dstSubpass,
                                                 VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                                                 VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                                                 VkDependencyFlags dependencyFlags) {
        VkSubpassDependency vkSubpassDependency{};
        vkSubpassDependency.srcSubpass = srcSubpass;
        vkSubpassDependency.dstSubpass = dstSubpass;
        vkSubpassDependency.srcStageMask = srcStageMask;
        vkSubpassDependency.dstStageMask = dstStageMask;
        vkSubpassDependency.srcAccessMask = srcAccessMask;
        vkSubpassDependency.dstAccessMask = dstAccessMask;
        vkSubpassDependency.dependencyFlags = dependencyFlags;
        return vkSubpassDependency;
    }

    inline VkRenderPassCreateInfo renderPassCreateInfo(const std::vector<VkAttachmentDescription> &attachments,
                                                       const std::vector<VkSubpassDependency> &dependencies,
                                                       const std::vector<VkSubpassDescription> &subpasses) {
        VkRenderPassCreateInfo vkRenderPassCreateInfo{};
        vkRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        vkRenderPassCreateInfo.attachmentCount = attachments.size();
        vkRenderPassCreateInfo.pAttachments = attachments.data();
        vkRenderPassCreateInfo.dependencyCount = dependencies.size();
        vkRenderPassCreateInfo.pDependencies = dependencies.data();
        vkRenderPassCreateInfo.subpassCount = subpasses.size();
        vkRenderPassCreateInfo.pSubpasses = subpasses.data();
        return vkRenderPassCreateInfo;
    }

    inline VkShaderModuleCreateInfo shaderModuleCreateInfo(const std::vector<char> &shaderBytes) {
        VkShaderModuleCreateInfo vkShaderModuleCreateInfo{};
        vkShaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        vkShaderModuleCreateInfo.codeSize = shaderBytes.size();
        vkShaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(shaderBytes.data());
        return vkShaderModuleCreateInfo;
    }

    inline VkImageViewCreateInfo imageViewCreateInfo(VkImage image, VkImageViewType viewType,
                                                     VkFormat format, VkComponentMapping components,
                                                     VkImageSubresourceRange subresourceRange) {
        VkImageViewCreateInfo vkImageViewCreateInfo{};
        vkImageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vkImageViewCreateInfo.image = image;
        vkImageViewCreateInfo.viewType = viewType;
        vkImageViewCreateInfo.format = format;
        vkImageViewCreateInfo.components = components;
        vkImageViewCreateInfo.subresourceRange = subresourceRange;
        return vkImageViewCreateInfo;
    }

    inline VkImageSubresourceRange imageSubresourceRange(VkImageAspectFlags aspectMask,
                                                         uint32_t baseMipLevel, uint32_t levelCount,
                                                         uint32_t baseArrayLayer, uint32_t layerCount) {
        VkImageSubresourceRange vkImageSubresourceRange{};
        vkImageSubresourceRange.aspectMask = aspectMask;
        vkImageSubresourceRange.baseMipLevel = baseMipLevel;
        vkImageSubresourceRange.levelCount = levelCount;
        vkImageSubresourceRange.baseArrayLayer = baseArrayLayer;
        vkImageSubresourceRange.layerCount = layerCount;
        return vkImageSubresourceRange;
    };

    inline VkFramebufferCreateInfo framebufferCreateInfo(VkFramebufferCreateFlags flags,
                                                         VkRenderPass renderPass, const std::vector<VkImageView> &attachments,
                                                         uint32_t width, uint32_t height, uint32_t layers) {
        VkFramebufferCreateInfo vkFramebufferCreateInfo{};
        vkFramebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        vkFramebufferCreateInfo.flags = flags;
        vkFramebufferCreateInfo.renderPass = renderPass;
        vkFramebufferCreateInfo.attachmentCount = attachments.size();
        vkFramebufferCreateInfo.pAttachments = attachments.data();
        vkFramebufferCreateInfo.width = width;
        vkFramebufferCreateInfo.height = height;
        vkFramebufferCreateInfo.layers = layers;
        return vkFramebufferCreateInfo;
    }

    inline VkRenderPassBeginInfo renderPassBeginInfo(VkRenderPass renderPass, VkFramebuffer framebuffer,
                                                     VkRect2D renderArea, const std::vector<VkClearValue> &clearValues) {
        VkRenderPassBeginInfo vkRenderPassBeginInfo{};
        vkRenderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        vkRenderPassBeginInfo.renderPass = renderPass;
        vkRenderPassBeginInfo.framebuffer = framebuffer;
        vkRenderPassBeginInfo.renderArea = renderArea;
        vkRenderPassBeginInfo.clearValueCount = clearValues.size();
        vkRenderPassBeginInfo.pClearValues = clearValues.data();
        return vkRenderPassBeginInfo;
    }

    inline VkPresentInfoKHR presentInfoKHR(const std::vector<VkSemaphore> &waitSemaphores,
                                           const std::vector<VkSwapchainKHR> &swapchains,
                                           const uint32_t *imageIndices,
                                           VkResult *pResults) {
        VkPresentInfoKHR vkPresentInfoKHR{};
        vkPresentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        vkPresentInfoKHR.waitSemaphoreCount = waitSemaphores.size();
        vkPresentInfoKHR.pWaitSemaphores = waitSemaphores.data();
        vkPresentInfoKHR.swapchainCount = swapchains.size();
        vkPresentInfoKHR.pSwapchains = swapchains.data();
        vkPresentInfoKHR.pImageIndices = imageIndices;
        vkPresentInfoKHR.pResults = pResults;
        return vkPresentInfoKHR;
    }

    inline VkBufferCopy bufferCopy(VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
        VkBufferCopy vkBufferCopy{};
        vkBufferCopy.size = size;
        vkBufferCopy.srcOffset = srcOffset;
        vkBufferCopy.dstOffset = dstOffset;
        return vkBufferCopy;
    }

    inline VkBufferMemoryBarrier bufferMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                                                     uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex,
                                                     VkBuffer buffer, VkDeviceSize offset, VkDeviceSize size) {
        VkBufferMemoryBarrier vkBufferMemoryBarrier{};
        vkBufferMemoryBarrier.sType = VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER;
        vkBufferMemoryBarrier.buffer = buffer;
        vkBufferMemoryBarrier.size = size;
        vkBufferMemoryBarrier.offset = offset;
        vkBufferMemoryBarrier.srcAccessMask = srcAccessMask;
        vkBufferMemoryBarrier.dstAccessMask = dstAccessMask;
        vkBufferMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
        vkBufferMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
        return vkBufferMemoryBarrier;
    }

    inline VkImageMemoryBarrier imageMemoryBarrier(VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask,
                                                   VkImageLayout oldLayout, VkImageLayout newLayout,
                                                   uint32_t srcQueueFamilyIndex, uint32_t dstQueueFamilyIndex,
                                                   VkImage image, VkImageSubresourceRange subresourceRange) {
        VkImageMemoryBarrier vkImageMemoryBarrier{};
        vkImageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        vkImageMemoryBarrier.srcAccessMask = srcAccessMask;
        vkImageMemoryBarrier.dstAccessMask = dstAccessMask;
        vkImageMemoryBarrier.oldLayout = oldLayout;
        vkImageMemoryBarrier.newLayout = newLayout;
        vkImageMemoryBarrier.srcQueueFamilyIndex = srcQueueFamilyIndex;
        vkImageMemoryBarrier.dstQueueFamilyIndex = dstQueueFamilyIndex;
        vkImageMemoryBarrier.image = image;
        vkImageMemoryBarrier.subresourceRange = subresourceRange;
        return vkImageMemoryBarrier;
    }

    inline VkBufferImageCopy bufferImageCopy(VkDeviceSize bufferOffset, uint32_t bufferRowLength, uint32_t bufferImageHeight,
                                             VkImageSubresourceLayers imageSubresource, VkOffset3D imageOffset, VkExtent3D imageExtent) {
        VkBufferImageCopy vkBufferImageCopy{};
        vkBufferImageCopy.bufferOffset = bufferOffset;
        vkBufferImageCopy.bufferRowLength = bufferRowLength;
        vkBufferImageCopy.bufferImageHeight = bufferImageHeight;
        vkBufferImageCopy.imageSubresource = imageSubresource;
        vkBufferImageCopy.imageOffset = imageOffset;
        vkBufferImageCopy.imageExtent = imageExtent;
        return vkBufferImageCopy;
    }

    inline VkImageSubresourceLayers imageSubresourceLayers(VkImageAspectFlags aspectMask,
                                                           uint32_t mipLevel,
                                                           uint32_t baseArrayLayer,
                                                           uint32_t layerCount) {
        VkImageSubresourceLayers vkImageSubresourceLayers{};
        vkImageSubresourceLayers.aspectMask = aspectMask;
        vkImageSubresourceLayers.mipLevel = mipLevel;
        vkImageSubresourceLayers.baseArrayLayer = baseArrayLayer;
        vkImageSubresourceLayers.layerCount = layerCount;
        return vkImageSubresourceLayers;
    }

    inline VkImageCreateInfo imageCreateInfo(VkImageCreateFlags flags, VkImageType imageType,
                                             VkFormat format, VkExtent3D extent,
                                             uint32_t mipLevels, uint32_t arrayLayers,
                                             VkSampleCountFlagBits samples, VkImageTiling tiling,
                                             VkImageUsageFlags usage, VkSharingMode sharingMode,
                                             const std::vector<uint32_t> &queueFamilyIndices, VkImageLayout initialLayout) {
        VkImageCreateInfo vkImageCreateInfo{};
        vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        vkImageCreateInfo.flags = flags;
        vkImageCreateInfo.imageType = imageType;
        vkImageCreateInfo.format = format;
        vkImageCreateInfo.extent = extent;
        vkImageCreateInfo.mipLevels = mipLevels;
        vkImageCreateInfo.arrayLayers = arrayLayers;
        vkImageCreateInfo.samples = samples;
        vkImageCreateInfo.tiling = tiling;
        vkImageCreateInfo.usage = usage;
        vkImageCreateInfo.sharingMode = sharingMode;
        vkImageCreateInfo.queueFamilyIndexCount = queueFamilyIndices.size();
        vkImageCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        vkImageCreateInfo.initialLayout = initialLayout;
        return vkImageCreateInfo;
    }

    inline VkImageViewCreateInfo imageCreateInfo(VkImageViewCreateFlags flags,
                                                 VkImage image, VkImageViewType viewType,
                                                 VkFormat format, VkComponentMapping components,
                                                 VkImageSubresourceRange subresourceRange) {
        VkImageViewCreateInfo vkImageCreateInfo{};
        vkImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vkImageCreateInfo.flags = flags;
        vkImageCreateInfo.image = image;
        vkImageCreateInfo.viewType = viewType;
        vkImageCreateInfo.format = format;
        vkImageCreateInfo.components = components;
        vkImageCreateInfo.subresourceRange = subresourceRange;
        return vkImageCreateInfo;
    }

    inline VkSamplerCreateInfo samplerCreateInfo(VkSamplerCreateFlags flags,
                                                 VkFilter magFilter, VkFilter minFilter,
                                                 VkSamplerMipmapMode mipmapMode, VkSamplerAddressMode addressModeU,
                                                 VkSamplerAddressMode addressModeV, VkSamplerAddressMode addressModeW,
                                                 float mipLodBias, VkBool32 anisotropyEnable,
                                                 float maxAnisotropy, VkBool32 compareEnable,
                                                 VkCompareOp compareOp, float minLod, float maxLod,
                                                 VkBorderColor borderColor, VkBool32 unnormalizedCoordinates) {
        VkSamplerCreateInfo vkSamplerCreateInfo{};
        vkSamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        vkSamplerCreateInfo.flags = flags;
        vkSamplerCreateInfo.magFilter = magFilter;
        vkSamplerCreateInfo.minFilter = minFilter;
        vkSamplerCreateInfo.mipmapMode = mipmapMode;
        vkSamplerCreateInfo.addressModeU = addressModeU;
        vkSamplerCreateInfo.addressModeV = addressModeV;
        vkSamplerCreateInfo.addressModeW = addressModeW;
        vkSamplerCreateInfo.mipLodBias = mipLodBias;
        vkSamplerCreateInfo.anisotropyEnable = anisotropyEnable;
        vkSamplerCreateInfo.maxAnisotropy = maxAnisotropy;
        vkSamplerCreateInfo.compareEnable = compareEnable;
        vkSamplerCreateInfo.compareOp = compareOp;
        vkSamplerCreateInfo.minLod = minLod;
        vkSamplerCreateInfo.maxLod = maxLod;
        vkSamplerCreateInfo.borderColor = borderColor;
        vkSamplerCreateInfo.unnormalizedCoordinates = unnormalizedCoordinates;
        return vkSamplerCreateInfo;
    }

    inline VkSamplerCreateInfo samplerCreateInfo() {
        VkSamplerCreateInfo vkSamplerCreateInfo{};
        vkSamplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        return vkSamplerCreateInfo;
    }

    inline VkDescriptorImageInfo descriptorImageInfo(VkSampler sampler, VkImageView imageView, VkImageLayout imageLayout) {
        VkDescriptorImageInfo vkDescriptorImageInfo;
        vkDescriptorImageInfo.sampler = sampler;
        vkDescriptorImageInfo.imageView = imageView;
        vkDescriptorImageInfo.imageLayout = imageLayout;
        return vkDescriptorImageInfo;
    }

    inline VkDescriptorBufferInfo descriptorBufferInfo(const VkBuffer& buffer, const VkDeviceSize& offset, const VkDeviceSize& range)
    {
        VkDescriptorBufferInfo vkDescriptorImageInfo;
        vkDescriptorImageInfo.buffer = buffer;
        vkDescriptorImageInfo.offset = offset;
        vkDescriptorImageInfo.range = range;
        return vkDescriptorImageInfo;
    }
}

#endif//VULKANBASE_CREATEINFOHELPERS_H
