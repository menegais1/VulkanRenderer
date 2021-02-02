//
// Created by menegais on 13/01/2021.
//

#ifndef VULKANBASE_PASSTHROUGHALLOCATOR_H
#define VULKANBASE_PASSTHROUGHALLOCATOR_H


#include "IMemoryAllocator.h"
#include "AllocationBlock.h"
#include "../Refactoring/PhysicalDeviceInfo.h"

class PassThroughAllocator : public IMemoryAllocator {
public:
    AllocationBlock vmalloc(VkDevice device, PhysicalDeviceInfo physicalDeviceInfo, VkMemoryRequirements requirements, VkMemoryPropertyFlagBits flags) override;

    bool vfree(VkDevice device, AllocationBlock block) override;

};


#endif //VULKANBASE_PASSTHROUGHALLOCATOR_H
