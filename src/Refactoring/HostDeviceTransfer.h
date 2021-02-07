//
// Created by menegais on 06/02/2021.
//

#ifndef VULKANBASE_HOSTDEVICETRANSFER_H
#define VULKANBASE_HOSTDEVICETRANSFER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "PhysicalDeviceInfo.h"

class HostDeviceTransfer {
public:
    HostDeviceTransfer(VkDevice vkDevice, PhysicalDeviceInfo physicalDeviceInfo);

private:
    VkCommandPool transferBuffersPool;
    VkCommandBufferAllocateInfo transferCommandBufferAllocateInfo;
    VkCommandBuffer transferCommandBuffer;
};


#endif //VULKANBASE_HOSTDEVICETRANSFER_H
