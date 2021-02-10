//
// Created by menegais on 02/02/2021.
//

#ifndef VULKANBASE_VULKANCREATEFUNCTIONS_H
#define VULKANBASE_VULKANCREATEFUNCTIONS_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

namespace vk {
    inline void VK_ASSERT(VkResult result) {
        if (result == VK_NOT_READY) {
            std::cerr << "VK_NOT_READY" << std::endl;
            exit(-1);
        } else if (result == VK_TIMEOUT) {
            std::cerr << "VK_TIMEOUT" << std::endl;
            exit(-1);
        } else if (result == VK_EVENT_SET) {
            std::cerr << "VK_EVENT_SET" << std::endl;
            exit(-1);
        } else if (result == VK_EVENT_RESET) {
            std::cerr << "VK_EVENT_RESET" << std::endl;
            exit(-1);
        } else if (result == VK_INCOMPLETE) {
            std::cerr << "VK_INCOMPLETE" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_OUT_OF_HOST_MEMORY) {
            std::cerr << "VK_ERROR_OUT_OF_HOST_MEMORY" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_OUT_OF_DEVICE_MEMORY) {
            std::cerr << "VK_ERROR_OUT_OF_DEVICE_MEMORY" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INITIALIZATION_FAILED) {
            std::cerr << "VK_ERROR_INITIALIZATION_FAILED" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_DEVICE_LOST) {
            std::cerr << "VK_ERROR_DEVICE_LOST" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_MEMORY_MAP_FAILED) {
            std::cerr << "VK_ERROR_MEMORY_MAP_FAILED" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_LAYER_NOT_PRESENT) {
            std::cerr << "VK_ERROR_LAYER_NOT_PRESENT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_EXTENSION_NOT_PRESENT) {
            std::cerr << "VK_ERROR_EXTENSION_NOT_PRESENT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_FEATURE_NOT_PRESENT) {
            std::cerr << "VK_ERROR_FEATURE_NOT_PRESENT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INCOMPATIBLE_DRIVER) {
            std::cerr << "VK_ERROR_INCOMPATIBLE_DRIVER" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_TOO_MANY_OBJECTS) {
            std::cerr << "VK_ERROR_TOO_MANY_OBJECTS" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_FORMAT_NOT_SUPPORTED) {
            std::cerr << "VK_ERROR_FORMAT_NOT_SUPPORTED" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_FRAGMENTED_POOL) {
            std::cerr << "VK_ERROR_FRAGMENTED_POOL" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_UNKNOWN) {
            std::cerr << "VK_ERROR_UNKNOWN" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_OUT_OF_POOL_MEMORY) {
            std::cerr << "VK_ERROR_OUT_OF_POOL_MEMORY" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_EXTERNAL_HANDLE) {
            std::cerr << "VK_ERROR_INVALID_EXTERNAL_HANDLE" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_FRAGMENTATION) {
            std::cerr << "VK_ERROR_FRAGMENTATION" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS) {
            std::cerr << "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_SURFACE_LOST_KHR) {
            std::cerr << "VK_ERROR_SURFACE_LOST_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_NATIVE_WINDOW_IN_USE_KHR) {
            std::cerr << "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_SUBOPTIMAL_KHR) {
            std::cerr << "VK_SUBOPTIMAL_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            std::cerr << "VK_ERROR_OUT_OF_DATE_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INCOMPATIBLE_DISPLAY_KHR) {
            std::cerr << "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_VALIDATION_FAILED_EXT) {
            std::cerr << "VK_ERROR_VALIDATION_FAILED_EXT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_SHADER_NV) {
            std::cerr << "VK_ERROR_INVALID_SHADER_NV" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INCOMPATIBLE_VERSION_KHR) {
            std::cerr << "VK_ERROR_INCOMPATIBLE_VERSION_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT) {
            std::cerr << "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_NOT_PERMITTED_EXT) {
            std::cerr << "VK_ERROR_NOT_PERMITTED_EXT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT) {
            std::cerr << "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT" << std::endl;
            exit(-1);
        } else if (result == VK_THREAD_IDLE_KHR) {
            std::cerr << "VK_THREAD_IDLE_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_THREAD_DONE_KHR) {
            std::cerr << "VK_THREAD_DONE_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_OPERATION_DEFERRED_KHR) {
            std::cerr << "VK_OPERATION_DEFERRED_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_OPERATION_NOT_DEFERRED_KHR) {
            std::cerr << "VK_OPERATION_NOT_DEFERRED_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_PIPELINE_COMPILE_REQUIRED_EXT) {
            std::cerr << "VK_OPERATION_NOT_DEFERRED_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_OUT_OF_POOL_MEMORY_KHR) {
            std::cerr << "VK_ERROR_OUT_OF_POOL_MEMORY_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR) {
            std::cerr << "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_FRAGMENTATION_EXT) {
            std::cerr << "VK_ERROR_FRAGMENTATION_EXT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_DEVICE_ADDRESS_EXT) {
            std::cerr << "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR) {
            std::cerr << "VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR" << std::endl;
            exit(-1);
        } else if (result == VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT) {
            std::cerr << "VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT" << std::endl;
            exit(-1);
        }
    }


    inline VkFence createFence(VkDevice vkDevice, VkFenceCreateInfo fenceCreateInfo) {
        VkFence vkFence;
        VK_ASSERT(vkCreateFence(vkDevice, &fenceCreateInfo, nullptr, &vkFence));
        return vkFence;
    }

    inline VkSemaphore createSemaphore(VkDevice vkDevice, VkSemaphoreCreateInfo semaphoreCreateInfo) {
        VkSemaphore vkSemaphore;
        VK_ASSERT(vkCreateSemaphore(vkDevice, &semaphoreCreateInfo, nullptr, &vkSemaphore));
        return vkSemaphore;
    }

    inline VkFramebuffer createFramebuffer(VkDevice vkDevice, VkFramebufferCreateInfo framebufferCreateInfo) {
        VkFramebuffer vkFramebuffer;
        VK_ASSERT(vkCreateFramebuffer(vkDevice, &framebufferCreateInfo, nullptr, &vkFramebuffer));
        return vkFramebuffer;
    }

    inline VkCommandPool createCommandPool(VkDevice vkDevice, VkCommandPoolCreateInfo commandPoolCreateInfo) {
        VkCommandPool vkCommandPool;
        VK_ASSERT(vkCreateCommandPool(vkDevice, &commandPoolCreateInfo, nullptr, &vkCommandPool));
        return vkCommandPool;
    }

    inline VkCommandBuffer *allocateCommandBuffers(VkDevice vkDevice, VkCommandBufferAllocateInfo commandBufferAllocateInfo, uint32_t commandBufferCount) {
        VkCommandBuffer *vkCommandBuffers = new VkCommandBuffer[commandBufferCount];
        VK_ASSERT(vkAllocateCommandBuffers(vkDevice, &commandBufferAllocateInfo, vkCommandBuffers));
        return vkCommandBuffers;
    }

    inline VkMemoryRequirements getBufferMemoryRequirements(VkDevice vkDevice, VkBuffer vkBuffer) {
        VkMemoryRequirements vkMemoryRequirements;
        vkGetBufferMemoryRequirements(vkDevice, vkBuffer, &vkMemoryRequirements);
        return vkMemoryRequirements;
    }

    inline VkMemoryRequirements getImageMemoryRequirements(VkDevice vkDevice, VkImage vkImage) {
        VkMemoryRequirements vkMemoryRequirements;
        vkGetImageMemoryRequirements(vkDevice, vkImage, &vkMemoryRequirements);
        return vkMemoryRequirements;
    }

    inline VkBuffer createBuffer(VkDevice vkDevice, VkBufferCreateInfo bufferCreateInfo) {
        VkBuffer vkBuffer;
        VK_ASSERT(vkCreateBuffer(vkDevice, &bufferCreateInfo, nullptr, &vkBuffer));
        return vkBuffer;
    }

    inline VkImage createImage(VkDevice vkDevice, VkImageCreateInfo vkImageCreateInfo) {
        VkImage vkImage;
        VK_ASSERT(vkCreateImage(vkDevice, &vkImageCreateInfo, nullptr, &vkImage));
        return vkImage;
    }

    inline VkImageView createImageView(VkDevice vkDevice, VkImageViewCreateInfo vkImageViewCreateInfo) {
        VkImageView vkImageView;
        VK_ASSERT(vkCreateImageView(vkDevice, &vkImageViewCreateInfo, nullptr, &vkImageView));
        return vkImageView;
    }

    inline VkSampler createSampler(VkDevice vkDevice, VkSamplerCreateInfo vkSamplerCreateInfo) {
        VkSampler vkSampler;
        VK_ASSERT(vkCreateSampler(vkDevice, &vkSamplerCreateInfo, nullptr, &vkSampler));
        return vkSampler;
    }

    inline VkRenderPass createRenderPass(VkDevice vkDevice, VkRenderPassCreateInfo vkRenderPassCreateInfo) {
        VkRenderPass vkRenderPass;
        VK_ASSERT(vkCreateRenderPass(vkDevice, &vkRenderPassCreateInfo, nullptr, &vkRenderPass));
        return vkRenderPass;
    }

    inline VkPipeline *createGraphicsPipelines(VkDevice vkDevice, VkPipelineCache pipelineCache, uint32_t createInfoCount, VkGraphicsPipelineCreateInfo *pCreateInfos) {
        VkPipeline *vkPipelines = new VkPipeline[createInfoCount];
        VK_ASSERT(vkCreateGraphicsPipelines(vkDevice, pipelineCache, createInfoCount, pCreateInfos, nullptr, vkPipelines));
        return vkPipelines;
    }

    inline VkDescriptorPool createDescriptorPool(VkDevice vkDevice, VkDescriptorPoolCreateInfo vkDescriptorPoolCreateInfo) {
        VkDescriptorPool vkDescriptorPool;
        VK_ASSERT(vkCreateDescriptorPool(vkDevice, &vkDescriptorPoolCreateInfo, nullptr, &vkDescriptorPool));
        return vkDescriptorPool;
    }

    inline VkDescriptorSet *allocateDescriptorSets(VkDevice vkDevice, int descriptorSetsCount, VkDescriptorSetAllocateInfo *vkDescriptorSetsAllocateInfo) {
        VkDescriptorSet *vkDescriptorSets = new VkDescriptorSet[descriptorSetsCount];
        VK_ASSERT(vkAllocateDescriptorSets(vkDevice, vkDescriptorSetsAllocateInfo, vkDescriptorSets));
        return vkDescriptorSets;
    }

    inline VkDescriptorSetLayout createDescriptorSetLayout(VkDevice vkDevice, VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutCreateInfo) {
        VkDescriptorSetLayout vkDescriptorSetLayout;
        VK_ASSERT(vkCreateDescriptorSetLayout(vkDevice, &vkDescriptorSetLayoutCreateInfo, nullptr, &vkDescriptorSetLayout));
        return vkDescriptorSetLayout;
    }

    inline VkCommandBufferBeginInfo commandBufferBeginInfo(VkCommandBufferUsageFlags flags) {
        VkCommandBufferBeginInfo vkCommandBufferBeginInfo{};
        vkCommandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkCommandBufferBeginInfo.flags = flags;
        return vkCommandBufferBeginInfo;
    }
}

#endif //VULKANBASE_VULKANCREATEFUNCTIONS_H
