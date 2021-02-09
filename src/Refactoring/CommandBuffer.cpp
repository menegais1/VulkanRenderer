//
// Created by menegais on 08/02/2021.
//

#include "CommandBuffer.h"
#include "VulkanCreateFunctions.h"
#include "CreateInfoHelpers.h"
#include <functional>
#include "CommandBufferUtils.h"

vk::CommandBuffer::CommandBuffer(VkDevice vkDevice, vk::Queue queue, VkCommandPool pool, VkCommandBufferLevel level) : queue(queue), vkDevice(vkDevice) {
    commandBuffer = vk::allocateCommandBuffers(vkDevice,
                                               vk::commandBufferAllocateInfo(pool, 1, level), 1)[0];
    fence = vk::createFence(vkDevice, vk::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT));
}


void vk::CommandBuffer::submitOneTime(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores,
                                      VkPipelineStageFlags *waitDstStageFlags, std::function<void(VkCommandBuffer)> callback) {
    vk::CommandBufferUtils::beginCommandBuffer(vkDevice, commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, {fence}, true);
    {
        callback(commandBuffer);
    }
    vk::CommandBufferUtils::submitCommandBuffer(queue.queue, commandBuffer, waitSemaphores, signalSemaphores, waitDstStageFlags, fence);
}
