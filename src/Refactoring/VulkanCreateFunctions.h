//
// Created by menegais on 02/02/2021.
//

#ifndef VULKANBASE_VULKANCREATEFUNCTIONS_H
#define VULKANBASE_VULKANCREATEFUNCTIONS_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>

namespace vk {


    VkFence createFence(VkDevice vkDevice, VkFenceCreateInfo fenceCreateInfo) {
        VkFence vkFence;
        VK_ASSERT(vkCreateFence(vkDevice, &fenceCreateInfo, nullptr, &vkFence));
        return vkFence;
    }

    VkSemaphore createSemaphore(VkDevice vkDevice, VkSemaphoreCreateInfo semaphoreCreateInfo) {
        VkSemaphore vkSemaphore;
        VK_ASSERT(vkCreateSemaphore(vkDevice, &semaphoreCreateInfo, nullptr, &vkSemaphore));
        return vkSemaphore;
    }

    VkFramebuffer createFramebuffer(VkDevice vkDevice, VkFramebufferCreateInfo framebufferCreateInfo) {
        VkFramebuffer vkFramebuffer;
        VK_ASSERT(vkCreateFramebuffer(vkDevice, &framebufferCreateInfo, nullptr, &vkFramebuffer));
        return vkFramebuffer;
    }

    VkCommandPool createCommandPool(VkDevice vkDevice, VkCommandPoolCreateInfo commandPoolCreateInfo) {
        VkCommandPool vkCommandPool;
        VK_ASSERT(vkCreateCommandPool(vkDevice, &commandPoolCreateInfo, nullptr, &vkCommandPool));
        return vkCommandPool;
    }

    VkCommandBuffer *allocateCommandBuffers(VkDevice vkDevice, VkCommandBufferAllocateInfo commandBufferAllocateInfo, uint32_t commandBufferCount) {
        VkCommandBuffer *vkCommandBuffers = new VkCommandBuffer[commandBufferCount];
        VK_ASSERT(vkAllocateCommandBuffers(vkDevice, &commandBufferAllocateInfo, vkCommandBuffers));
        return vkCommandBuffers;
    }

    VkMemoryRequirements getBufferMemoryRequirements(VkDevice vkDevice, VkBuffer vkBuffer) {
        VkMemoryRequirements vkMemoryRequirements;
        vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &vkMemoryRequirements);
        return vkMemoryRequirements;
    }

    VkMemoryRequirements getImageMemoryRequirements(VkDevice vkDevice, VkImage vkImage) {
        VkMemoryRequirements vkMemoryRequirements;
        vkGetImageMemoryRequirements(vkDevice, vkImage, &vkMemoryRequirements);
        return vkMemoryRequirements;
    }

    VkBuffer createBuffer(VkDevice vkDevice, VkBufferCreateInfo bufferCreateInfo) {
        VkBuffer vkBuffer;
        VK_ASSERT(vkCreateBuffer(vkDevice, &bufferCreateInfo, nullptr, &vkBuffer));
        return vkBuffer;
    }
}

#endif //VULKANBASE_VULKANCREATEFUNCTIONS_H
