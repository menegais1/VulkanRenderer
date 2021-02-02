//
// Created by menegais on 13/01/2021.
//

#include "VideoMemoryAllocator.h"

VideoMemoryAllocator::VideoMemoryAllocator(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, IMemoryAllocator *memoryAllocator) : device(device), physicalDeviceInfo(physicalDeviceInfo),
                                                                                                                                        memoryAllocator(memoryAllocator) {

}

AllocationBlock VideoMemoryAllocator::vmalloc(VkMemoryRequirements requirements, VkMemoryPropertyFlagBits flags) {
    AllocationBlock newAllocation = memoryAllocator->vmalloc(device, physicalDeviceInfo, requirements, flags);
    freeMemory -= newAllocation.vkSize;
    allocatedMemory += newAllocation.vkSize;
    currentAllocations += 1;
    return newAllocation;
}

bool VideoMemoryAllocator::vfree(AllocationBlock block) {
    memoryAllocator->vfree(device, block);
    freeMemory += block.vkSize;
    allocatedMemory -= block.vkSize;
}

VideoMemoryAllocator::VideoMemoryAllocator() {}
