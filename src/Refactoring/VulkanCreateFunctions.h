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

    VkFramebuffer createFramebuffer(VkDevice vkDevice, VkFramebufferCreateInfo framebufferCreateInfo){
        VkFramebuffer vkFramebuffer;
        VK_ASSERT(vkCreateFramebuffer(vkDevice, &framebufferCreateInfo, nullptr, &vkFramebuffer));
        return vkFramebuffer;
    }
}

#endif //VULKANBASE_VULKANCREATEFUNCTIONS_H
