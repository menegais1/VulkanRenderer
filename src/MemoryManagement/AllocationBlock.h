//
// Created by menegais on 12/01/2021.
//

#ifndef VULKANBASE_ALLOCATIONBLOCK_H
#define VULKANBASE_ALLOCATIONBLOCK_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

class AllocationBlock {
public:
    VkDeviceMemory vkDeviceMemory;
    VkDeviceSize vkSize;
    VkDeviceSize vkOffset;
};


#endif //VULKANBASE_ALLOCATIONBLOCK_H
