//
// Created by menegais on 24/11/2020.
//

#ifndef VULKANBASE_COMMANDBUFFERUTILS_H
#define VULKANBASE_COMMANDBUFFERUTILS_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "../VulkanStructures.h"
#include <vector>
#include <iostream>

namespace vk {

    class CommandBufferUtils {
    public:

        static VkCommandPool createCommandPool(const VkDevice vkDevice, const int queueFamilyIndex, VkCommandPoolCreateFlags flags);

        static std::vector<VkCommandBuffer> createCommandBuffers(const VkDevice vkDevice,
                                                                 VkCommandPool const vkCommandPool,
                                                                 const int commandBufferCount, VkCommandBufferLevel level);

        /*
         * Begin a command buffer and wait for fences, if any provided, reset fences automatically
         */
        static void beginCommandBuffer(const VkDevice vkDevice, VkCommandBuffer commandBuffer,
                                       VkCommandBufferUsageFlags flags,
                                       std::vector<VkFence> fence = std::vector<VkFence>(0),
                                       bool resetFences = true);

        /*
         * End and submit a single command buffer
         */
        static void
        submitCommandBuffer(VkQueue queue,
                            VkCommandBuffer commandBuffer,
                            std::vector<VkSemaphore> waitSemaphores,
                            std::vector<VkSemaphore> signalSemaphores,
                            VkPipelineStageFlags *waitDstStageFlags, VkFence fence);

        static void
        waitForFences(const VkDevice vkDevice, std::vector<VkFence> fences, bool resetFences = true);
    };

}


#endif //VULKANBASE_COMMANDBUFFERUTILS_H
