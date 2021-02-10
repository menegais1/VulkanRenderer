//
// Created by menegais on 08/02/2021.
//

#ifndef VULKANBASE_COMMANDBUFFER_H
#define VULKANBASE_COMMANDBUFFER_H


#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include "Buffer.h"
#include "Queue.h"
#include <functional>

namespace vk {
    class CommandBuffer {
    public:
        VkCommandBuffer commandBuffer;
        VkFence fence;
        Queue queue;
        VkDevice vkDevice;
        VkCommandPool pool;

        CommandBuffer();

        CommandBuffer(VkDevice vkDevice, vk::Queue queue, VkCommandPool pool, VkCommandBufferLevel level);
//
//        void begin(VkCommandBufferUsageFlags flags);
//
//        void submit(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores, VkPipelineStageFlags *waitDstStageFlags);

//        void submitOneTime(std::vector<VkFence> fences, bool resetFences, std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores,
//                           VkPipelineStageFlags *waitDstStageFlags, std::function<void(VkCommandBuffer)> callback);

        void submitOneTime(std::vector<VkSemaphore> waitSemaphores, std::vector<VkSemaphore> signalSemaphores,
                           VkPipelineStageFlags *waitDstStageFlags, std::function<void(VkCommandBuffer)> callback);

//        void waitForFences(const std::vector<VkFence> &fences, bool resetFences);
    };
}

#endif //VULKANBASE_COMMANDBUFFER_H
