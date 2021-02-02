//
// Created by menegais on 13/01/2021.
//

#include "PassThroughAllocator.h"
#include "MemoryAllocationHelpers.h"

AllocationBlock PassThroughAllocator::vmalloc(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, VkMemoryRequirements requirements, VkMemoryPropertyFlagBits flags) {
    VkMemoryAllocateInfo vkMemoryAllocateInfo{};
    vkMemoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    vkMemoryAllocateInfo.allocationSize = requirements.size;
    vkMemoryAllocateInfo.memoryTypeIndex = vulkanGetMemoryTypeIndex(physicalDeviceInfo, requirements.memoryTypeBits, flags);

    AllocationBlock newBlock{};
    newBlock.vkSize = requirements.size;
    newBlock.vkOffset = 0;
    vkAllocateMemory(device, &vkMemoryAllocateInfo, nullptr, &newBlock.vkDeviceMemory);

    return newBlock;
}

bool PassThroughAllocator::vfree(VkDevice device, AllocationBlock block) {
    vkFreeMemory(device, block.vkDeviceMemory, nullptr);
    return true;
}
