//
// Created by menegais on 10/11/2020.
//

#ifndef VULKANBASE_VULKANSTRUCTURE_H
#define VULKANBASE_VULKANSTRUCTURE_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include "MemoryManagement/AllocationBlock.h"

//#define VK_ASSERT(VK_RESULT) if(VK_RESULT != VK_SUCCESS) throw std::runtime_error("ERROR ON VKRESULT");



struct SwapchainReferences {
    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
};

struct CommandBufferStructure {
    VkCommandBuffer commandBuffer;
    VkFence bufferAvaibleFence;
    VkQueue queue;
    uint32_t queueFamilyIndex;
};

struct Buffer {
    VkBuffer buffer;
    VkDeviceSize size;
    AllocationBlock memory;
};

struct Texture2D {
    void *data;
    uint32_t width;
    uint32_t height;
    VkImage image;
    VkImageView imageView;
    VkSampler sampler;
    AllocationBlock memory;

};


#endif