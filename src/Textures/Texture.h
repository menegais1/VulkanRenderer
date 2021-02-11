//
// Created by Lorenzo on 2/11/12021.
//

#ifndef VULKAN_BASE_TEXTURE_H
#define VULKAN_BASE_TEXTURE_H

#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "../Refactoring/HostDeviceTransfer.h"
#include "../Refactoring/CommandBuffer.h"

class Texture
{
public:
    Texture(const VkDevice& vkDevice, const std::string& path, vk::HostDeviceTransfer& hostDeviceTransfer,
            const std::vector<uint32_t>& graphicsAndTransferQueues, const vk::CommandBuffer& commandBuffer);
    VkImage image;
    VkSampler sampler;
    VkImageView imageView;
private:
};


#endif //VULKAN_BASE_TEXTURE_H
