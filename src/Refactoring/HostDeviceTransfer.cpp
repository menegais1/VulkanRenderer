//
// Created by menegais on 06/02/2021.
//

#include "HostDeviceTransfer.h"
#include "VulkanCreateFunctions.h"
#include "CreateInfoHelpers.h"

HostDeviceTransfer::HostDeviceTransfer(VkDevice vkDevice, PhysicalDeviceInfo physicalDeviceInfo) {
    transferBuffersPool = vk::createCommandPool(vkDevice, vk::commandPoolCreateInfo(physicalDeviceInfo.queueFamilies.transferFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT));
    transferCommandBufferAllocateInfo = vk::commandBufferAllocateInfo(transferBuffersPool, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    transferCommandBuffer = vk::allocateCommandBuffers(vkDevice, transferCommandBufferAllocateInfo, 1)[0];
}
