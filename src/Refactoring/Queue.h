//
// Created by menegais on 07/02/2021.
//

#ifndef VULKANBASE_QUEUE_H
#define VULKANBASE_QUEUE_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <cstdint>

namespace vk {
    class Queue {
    public:
        VkQueue queue;
        uint32_t queueFamily;
        uint32_t queueIndex;

        Queue(VkDevice vkDevice, uint32_t queueFamily, uint32_t queueIndex);
    };

}
#endif //VULKANBASE_QUEUE_H
