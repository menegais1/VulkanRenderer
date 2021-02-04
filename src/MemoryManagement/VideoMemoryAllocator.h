//
// Created by menegais on 13/01/2021.
//

#ifndef VULKANBASE_VIDEOMEMORYALLOCATOR_H
#define VULKANBASE_VIDEOMEMORYALLOCATOR_H


#include <cstdint>
#include "AllocationBlock.h"
#include "IMemoryAllocator.h"
#include "../Refactoring/PhysicalDeviceInfo.h"

class VideoMemoryAllocator {
public:

    VideoMemoryAllocator();

    VideoMemoryAllocator(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, IMemoryAllocator *memoryAllocator);

    AllocationBlock vmalloc(VkMemoryRequirements requirements, VkMemoryPropertyFlags flags);

    bool vfree(AllocationBlock block);

private:
    IMemoryAllocator *memoryAllocator;
    VkDevice device;
    PhysicalDeviceInfo physicalDeviceInfo;
    uint32_t currentAllocations;
    uint32_t allocatedMemory;
    uint32_t freeMemory;
};


#endif //VULKANBASE_VIDEOMEMORYALLOCATOR_H
