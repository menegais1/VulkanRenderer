//
// Created by menegais on 24/11/2020.
//

#include "CommandBufferUtils.h"
#include "CreateInfoHelpers.h"
#include "VulkanCreateFunctions.h"

void vk::CommandBufferUtils::beginCommandBuffer(VkDevice vkDevice, VkCommandBuffer commandBuffer,
                                                VkCommandBufferUsageFlags flags, std::vector<VkFence> fences,
                                                bool resetFences) {
    VkCommandBufferBeginInfo bufferBegin{};
    bufferBegin.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bufferBegin.flags = flags;
    waitForFences(vkDevice, fences, resetFences);
    vk::VK_ASSERT(vkBeginCommandBuffer(commandBuffer, &bufferBegin));
}

void vk::CommandBufferUtils::submitCommandBuffer(VkQueue queue,
                                                 VkCommandBuffer commandBuffer,
                                                 std::vector<VkSemaphore> waitSemaphores,
                                                 std::vector<VkSemaphore> signalSemaphores,
                                                 VkPipelineStageFlags *waitDstStageFlags, VkFence fence) {
    vk::VK_ASSERT(vkEndCommandBuffer(commandBuffer));
    std::vector<VkCommandBuffer> submitBuffers = {commandBuffer};
    VkSubmitInfo transferSubmitInfo = vk::submitInfo(submitBuffers, waitSemaphores, signalSemaphores, waitDstStageFlags);
    vk::VK_ASSERT(vkQueueSubmit(queue, 1, &transferSubmitInfo, fence));
}

void vk::CommandBufferUtils::waitForFences(const VkDevice vkDevice, std::vector<VkFence> fences,
                                           bool resetFences) {
    if (!fences.empty()) {
        vk::VK_ASSERT(vkWaitForFences(vkDevice, fences.size(), fences.data(), VK_TRUE, UINT64_MAX));
        if (resetFences)
            vk::VK_ASSERT(vkResetFences(vkDevice, 1, fences.data()));
    }
}
