//
// Created by menegais on 13/01/2021.
//

#ifndef VULKANBASE_IMEMORYALLOCATOR_H
#define VULKANBASE_IMEMORYALLOCATOR_H


#include "AllocationBlock.h"
#include "../Refactoring/PhysicalDeviceInfo.h"
class IMemoryAllocator {
public:
    virtual AllocationBlock vmalloc(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, VkMemoryRequirements requirements, VkMemoryPropertyFlags flags) = 0;

    virtual bool vfree(VkDevice device, AllocationBlock block) = 0;
};


#endif //VULKANBASE_IMEMORYALLOCATOR_H
