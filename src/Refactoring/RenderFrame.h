//
// Created by menegais on 02/02/2021.
//

#ifndef VULKANBASE_RENDERFRAME_H
#define VULKANBASE_RENDERFRAME_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

struct RenderFrame {
    VkCommandBuffer commandBuffer;
    VkSemaphore imageReadySemaphore;
    VkSemaphore presentationReadySemaphore;
    VkFramebuffer frameBuffer;
    VkFence bufferFinishedFence;
};

#endif //VULKANBASE_RENDERFRAME_H
