//
// Created by menegais on 07/02/2021.
//

#include "Image2D.h"
#include "../VulkanCreateFunctions.h"
#include "../CreateInfoHelpers.h"
#include "../../MemoryManagement/VMA.h"

vk::Image2D::Image2D(VkDevice vkDevice, uint32_t width, uint32_t height, uint32_t channels, uint32_t bytesPerChannel, VkFormat format,
                     VkImageUsageFlags usage, VkSharingMode sharingMode, const std::vector<uint32_t> &queueFamilyIndices,
                     VkMemoryPropertyFlags memoryProperties) : width(width), height(height), channels(channels), bytesPerChannel(bytesPerChannel), format(format) {
    image = vk::createImage(vkDevice, vk::imageCreateInfo(0, VK_IMAGE_TYPE_2D, format,
                                                          vk::extent3D(width, height, 1), 1, 1, VK_SAMPLE_COUNT_1_BIT,
                                                          VK_IMAGE_TILING_OPTIMAL, usage, sharingMode, queueFamilyIndices, VK_IMAGE_LAYOUT_UNDEFINED));
    VkMemoryRequirements requirements = vk::getImageMemoryRequirements(vkDevice, image);
    memory = VMA::getInstance().vmalloc(requirements, memoryProperties);
    vkBindImageMemory(vkDevice, image, memory.vkDeviceMemory, memory.vkOffset);
}
