//
// Created by menegais on 03/02/2021.
//

#ifndef VULKANBASE_BUFFER_H
#define VULKANBASE_BUFFER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include "../MemoryManagement/AllocationBlock.h"

namespace vk {

    class Buffer {
    public:

        Buffer();

        Buffer(VkDevice vkDevice, VkDeviceSize memorySize, const std::vector<uint32_t>& queues,
               VkBufferUsageFlags usage, VkSharingMode sharingMode, VkBufferCreateFlags flags,
               VkMemoryPropertyFlags memoryPropertyFlags);

        VkBuffer buffer;
        VkDeviceSize size;
        AllocationBlock memory;
    };
}

#endif //VULKANBASE_BUFFER_H
