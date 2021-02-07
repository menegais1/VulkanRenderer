//
// Created by menegais on 06/02/2021.
//

#ifndef VULKANBASE_HOSTDEVICETRANSFER_H
#define VULKANBASE_HOSTDEVICETRANSFER_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "PhysicalDeviceInfo.h"
#include "Buffer.h"
#include "Queue.h"
#include <functional>

namespace vk {
    class HostDeviceTransfer {
    public:
        VkFence transferCompleted;

        HostDeviceTransfer(VkDevice vkDevice, vk::Queue transferQueue);

        void transferBuffer(uint32_t dataSize, void *data, vk::Buffer dstBuffer, std::function<void(VkCommandBuffer commandBuffer)> callback);

    private:
        VkDevice vkDevice;
        PhysicalDeviceInfo physicalDeviceInfo;
        VkCommandPool transferBuffersPool;
        VkCommandBufferAllocateInfo transferCommandBufferAllocateInfo;
        VkCommandBuffer transferCommandBuffer;
        VkSemaphore transferExecuting;
        vk::Queue transferQueue;
        vk::Buffer stagingBuffer;
        void *mappedMemory;
        bool bufferAllocated;

        void allocateStagingBuffer(VkDeviceSize size);
    };
}

#endif //VULKANBASE_HOSTDEVICETRANSFER_H
