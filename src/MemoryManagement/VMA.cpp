//
// Created by menegais on 06/02/2021.
//

#include "AllocationBlock.h"
#include "VMA.h"

AllocationBlock VMA::vmalloc(VkMemoryRequirements requirements, VkMemoryPropertyFlags flags) {
    if (!isInitialized) {
        std::cerr << "Video Memory Allocator Unitialized" << std::endl;
        exit(-1);
    }
    return videoMemoryAllocator.vmalloc(requirements, flags);
}

bool VMA::vfree(AllocationBlock block) {
    if (!isInitialized) {
        std::cerr << "Video Memory Allocator Unitialized" << std::endl;
        exit(-1);
    }
    return videoMemoryAllocator.vfree(block);
}

void VMA::initialize(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, IMemoryAllocator *memoryAllocator) {
    if (isInitialized) {
        std::cerr << "Video Memory Allocator Can't be initialized twice" << std::endl;
        exit(-1);
    }
    videoMemoryAllocator = VideoMemoryAllocator(device, physicalDeviceInfo, memoryAllocator);
    isInitialized = true;
}

VMA& VMA::getInstance() {
    static VMA vma;
    return vma;
}
