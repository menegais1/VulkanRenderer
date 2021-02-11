//
// Created by menegais on 08/02/2021.
//

#include "CommandBuffer.h"
#include "VulkanCreateFunctions.h"
#include "CreateInfoHelpers.h"
#include <functional>
#include "CommandBufferUtils.h"

vk::CommandBuffer::CommandBuffer(VkDevice vkDevice, vk::Queue queue, VkCommandPool pool, VkCommandBufferLevel level) : queue(queue), vkDevice(vkDevice), pool(pool) {
    commandBuffer = vk::allocateCommandBuffers(vkDevice,
                                               vk::commandBufferAllocateInfo(pool, 1, level), 1)[0];
    fence = vk::createFence(vkDevice, vk::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT));
}

vk::CommandBuffer::CommandBuffer() {

}

void vk::CommandBuffer::submitOneTime(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores,
                                      VkPipelineStageFlags *waitDstStageFlags, const std::function<void(VkCommandBuffer)>& callback) const
{
    vk::CommandBufferUtils::beginCommandBuffer(vkDevice, commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, {fence}, true);
    {
        callback(commandBuffer);
    }
    vk::CommandBufferUtils::submitCommandBuffer(queue.queue, commandBuffer, waitSemaphores, signalSemaphores, waitDstStageFlags, fence);
}
//
//void vk::CommandBuffer::submitOneTime(std::vector<VkFence> fences, bool resetFences, std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores,
//                                      VkPipelineStageFlags *waitDstStageFlags, std::function<void(VkCommandBuffer)> callback) {
//    vk::CommandBufferUtils::waitForFences(vkDevice, fences, resetFences);
//    vk::CommandBufferUtils::beginCommandBuffer(vkDevice, commandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT, {fence}, true);
//    {
//        callback(commandBuffer);
//    }
//    vk::CommandBufferUtils::submitCommandBuffer(queue.queue, commandBuffer, waitSemaphores, signalSemaphores, waitDstStageFlags, fence);
//}

//void vk::CommandBuffer::waitForFences(const std::vector<VkFence> &fences, bool resetFences) {
//    vk::VK_ASSERT(vkWaitForFences(vkDevice, fences.size(), fences.data(), VK_TRUE, UINT64_MAX));
//    if (resetFences)
//        vk::VK_ASSERT(vkResetFences(vkDevice, 1, fences.data()));
//}

//void vk::CommandBuffer::submit(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores, VkPipelineStageFlags *waitDstStageFlags) {
//    vk::CommandBufferUtils::submitCommandBuffer(queue.queue, commandBuffer, waitSemaphores, signalSemaphores, waitDstStageFlags, fence);
//}
//
//void vk::CommandBuffer::begin(VkCommandBufferUsageFlags flags) {
//    vk::CommandBufferUtils::beginCommandBuffer(vkDevice, commandBuffer, flags, {fence}, true);
//}
