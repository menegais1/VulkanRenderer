//
// Created by menegais on 13/01/2021.
//

#ifndef VULKANBASE_MEMORYALLOCATIONHELPERS_H
#define VULKANBASE_MEMORYALLOCATIONHELPERS_H

#include "../Refactoring/PhysicalDeviceInfo.h"

int vulkanGetMemoryTypeIndex(PhysicalDeviceInfo physicalDeviceInfo, uint32_t memoryTypeBits,
                             VkMemoryPropertyFlags flagBits) {
    for (int i = 0; i < physicalDeviceInfo.physicalDeviceMemoryProperties.memoryTypeCount; ++i) {
        VkMemoryType memoryType = physicalDeviceInfo.physicalDeviceMemoryProperties.memoryTypes[i];
        if ((memoryTypeBits & (1 << i)) && (memoryType.propertyFlags & flagBits) == flagBits) {
            return i;
        }
    }
    return -1;
}

#endif //VULKANBASE_MEMORYALLOCATIONHELPERS_H
