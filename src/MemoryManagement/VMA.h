//
// Created by menegais on 06/02/2021.
//

#ifndef VULKANBASE_VMA_H
#define VULKANBASE_VMA_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <iostream>
#include "../Refactoring/PhysicalDeviceInfo.h"
#include "VideoMemoryAllocator.h"

class VMA {
public:
    AllocationBlock vmalloc(VkMemoryRequirements requirements, VkMemoryPropertyFlags flags);

    bool vfree(AllocationBlock block);

    void initialize(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, IMemoryAllocator *memoryAllocator);

    static VMA& getInstance();
private:
    bool isInitialized = false;
    VideoMemoryAllocator videoMemoryAllocator;
};


#endif //VULKANBASE_VMA_H
