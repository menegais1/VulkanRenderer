//
// Created by menegais on 03/02/2021.
//

#ifndef VULKANBASE_BUFFER_H
#define VULKANBASE_BUFFER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "../MemoryManagement/AllocationBlock.h"

struct Buffer {
    VkBuffer buffer;
    VkDeviceSize size;
    AllocationBlock memory;
};

#endif //VULKANBASE_BUFFER_H
