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

        /// Method for copying a buffer from CPU to GPU, the VkSharingMode needs to be VK_SHARING_MODE_CONCURRENT
        void transferBuffer(uint32_t dataSize, void *data, vk::Buffer dstBuffer);

        /// Method for copying an image from CPU to GPU using a Buffer, the VkSharingMode needs to be VK_SHARING_MODE_CONCURRENT.
        void transferImageFromBuffer(uint32_t width, uint32_t height, uint32_t channels, uint32_t bytesPerChannel,
                                     void *data, VkImage dstImage, VkImageLayout dstImageLayout);

        void submitOneTimeTransferBuffer(const std::function<void(VkCommandBuffer transferBuffer)>& callback) const;
    private:
        VkDevice vkDevice;
        PhysicalDeviceInfo physicalDeviceInfo;
        VkCommandPool transferBuffersPool;
        VkCommandBufferAllocateInfo transferCommandBufferAllocateInfo;
        VkCommandBuffer transferCommandBuffer;
        vk::Queue transferQueue;
        vk::Buffer stagingBuffer;
        void *mappedMemory;
        bool bufferAllocated;

        void allocateStagingBuffer(VkDeviceSize size);
    };
}

#endif //VULKANBASE_HOSTDEVICETRANSFER_H
