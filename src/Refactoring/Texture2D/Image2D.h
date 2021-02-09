//
// Created by menegais on 07/02/2021.
//

#ifndef VULKANBASE_IMAGE2D_H
#define VULKANBASE_IMAGE2D_H

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <cstdint>
#include "../../MemoryManagement/AllocationBlock.h"
#include <vector>

namespace vk {
    class Image2D {
    public:
        uint32_t width;
        uint32_t height;
        VkFormat format;
        VkImage image;
        uint32_t channels;

        uint32_t bytesPerChannel;

        AllocationBlock memory;

        Image2D(VkDevice vkDevice, uint32_t width, uint32_t height, uint32_t channels, uint32_t bytesPerChannel, VkFormat format,
                VkImageUsageFlags usage, VkSharingMode sharingMode, const std::vector<uint32_t> &queueFamilyIndices,
                VkMemoryPropertyFlags memoryProperties);
    };
}

#endif //VULKANBASE_IMAGE2D_H
