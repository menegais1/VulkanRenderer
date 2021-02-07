//
// Created by menegais on 06/02/2021.
//

#include <cstring>
#include "HostDeviceTransfer.h"
#include "VulkanCreateFunctions.h"
#include "CreateInfoHelpers.h"
#include "CommandBufferUtils.h"
#include "Queue.h"

vk::HostDeviceTransfer::HostDeviceTransfer(VkDevice vkDevice, vk::Queue transferQueue) : vkDevice(vkDevice), transferQueue(transferQueue) {
    transferBuffersPool = vk::createCommandPool(vkDevice, vk::commandPoolCreateInfo(transferQueue.queueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT));
    transferCommandBufferAllocateInfo = vk::commandBufferAllocateInfo(transferBuffersPool, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    transferCommandBuffer = vk::allocateCommandBuffers(vkDevice, transferCommandBufferAllocateInfo, 1)[0];
    transferCompleted = vk::createFence(vkDevice, vk::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT));
//    transferExecuting = vk::createSemaphore(vkDevice, vk::semaphoreCreateInfo());
    // Allocate a default buffer of 1 MB
    allocateStagingBuffer(1024 * 1024 * 10);
}

void vk::HostDeviceTransfer::transferBuffer(uint32_t dataSize, void *data, vk::Buffer dstBuffer, std::function<void(VkCommandBuffer commandBuffer)> callback) {
    vk::CommandBufferUtils::waitForFences(vkDevice, {transferCompleted}, true);

    if (dataSize > stagingBuffer.size) {
        allocateStagingBuffer(dataSize * 1.1);
    }

    memcpy(mappedMemory, data, dataSize);

    VkPipelineStageFlags waitDstStageFlags = VK_PIPELINE_STAGE_TRANSFER_BIT;
    vk::CommandBufferUtils::beginCommandBuffer(vkDevice, transferCommandBuffer, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
    {
        VkBufferCopy vkBufferCopy = vk::bufferCopy(dataSize, 0, 0);
        vkCmdCopyBuffer(transferCommandBuffer, stagingBuffer.buffer, dstBuffer.buffer, 1, &vkBufferCopy);
        callback(transferCommandBuffer);
    }
    vk::CommandBufferUtils::submitCommandBuffer(transferQueue.queue, transferCommandBuffer, {}, {}, &waitDstStageFlags, transferCompleted);
}

void vk::HostDeviceTransfer::allocateStagingBuffer(VkDeviceSize size) {
    if (bufferAllocated) {
        vkDestroyBuffer(vkDevice, stagingBuffer.buffer, nullptr);
    }
    std::cout << "Allocating buffer of size " << (size / 1024 / 1024) << "(MB)" << std::endl;
    stagingBuffer = vk::Buffer(vkDevice, size, {physicalDeviceInfo.queueFamilies.transferFamily},
                               VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE, 0,
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    vkMapMemory(vkDevice, stagingBuffer.memory.vkDeviceMemory, stagingBuffer.memory.vkOffset, size, 0, &mappedMemory);

}
